#!/usr/bin/env bash
# Bateria de benchmarks — Trabalho 1 (remoção de ruído com OpenMP)
# Uso: ./run_tests.sh [imagem] [seq_bin] [par_bin]
# Padrão: data/entrada_grande.ppm, ./sequencial, ./paralelo

set -euo pipefail

IMG="${1:-data/entrada_grande.ppm}"
SEQ_BIN="${2:-./sequencial}"
PAR_BIN="${3:-./paralelo}"

LOG_DIR="data/logs"
THREADS=(2 4 8 12 16)
RUNS=5

mkdir -p "$LOG_DIR"
IMG_NAME=$(basename "$IMG" .ppm)

# ---- Sanidade ----
[[ -x "$SEQ_BIN" ]] || { echo "Erro: '$SEQ_BIN' não executável."; exit 1; }
[[ -x "$PAR_BIN" ]] || { echo "Erro: '$PAR_BIN' não executável."; exit 1; }
[[ -f "$IMG"     ]] || { echo "Erro: imagem '$IMG' não encontrada."; exit 1; }

# ---- Governor (aviso) ----
GOV=$(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor 2>/dev/null || echo "?")
[[ "$GOV" != "performance" ]] && {
    echo "AVISO: governor='$GOV'. Recomendado:"
    echo "  echo performance | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor"
    echo
}

# ---- Sequencial ----
SEQ_LOG="$LOG_DIR/seq_${IMG_NAME}.log"
: > "$SEQ_LOG"
echo "=== Sequencial: $IMG ($RUNS execuções) ===" | tee -a "$SEQ_LOG"
for r in $(seq 1 "$RUNS"); do
    echo "--- seq run $r ---" | tee -a "$SEQ_LOG"
    /usr/bin/time -a -o "$SEQ_LOG" "$SEQ_BIN" "$IMG" "data/seq_out.ppm" 2>&1 | tee -a "$SEQ_LOG"
done

# ---- Paralelo ----
PAR_LOG="$LOG_DIR/par_${IMG_NAME}.log"
: > "$PAR_LOG"
echo "=== Paralelo: $IMG ===" | tee -a "$PAR_LOG"
for t in "${THREADS[@]}"; do
    for r in $(seq 1 "$RUNS"); do
        echo "--- par t=$t run=$r ---" | tee -a "$PAR_LOG"
        OMP_NUM_THREADS="$t" /usr/bin/time -a -o "$PAR_LOG" \
            "$PAR_BIN" "$IMG" "data/par_out_${t}t.ppm" 2>&1 | tee -a "$PAR_LOG"
    done
done

echo
echo "Logs em $LOG_DIR/"
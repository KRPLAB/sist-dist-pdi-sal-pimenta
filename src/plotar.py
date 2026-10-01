#!/usr/bin/env python3
"""
Gera os gráficos de desempenho para o Trabalho 1 (remoção de ruído com OpenMP).
Uso: python3 src/plotar.py
"""
import matplotlib.pyplot as plt
import numpy as np
import os

os.makedirs("figuras", exist_ok=True)

# ============================================================================
# Dados medidos (média de 5 execuções, governor=performance)
# ============================================================================
P = np.array([2, 4, 8, 12, 16])

# Tempos sequenciais (referência T_s)
T_s = {
    "Pequena":       0.249002,
    "Média":        11.859412,
    "Grande":       86.605967,
    "Extra Grande": 616.853455,
}

# Tempos paralelos T_p(p) para cada imagem (colunas: p=2,4,8,12,16)
T_p = {
    "Pequena":       np.array([0.119158, 0.060831, 0.038521, 0.031885, 0.028198]),
    "Média":         np.array([5.624274, 2.929537, 1.590711, 1.475008, 1.249827]),
    "Grande":        np.array([42.275675, 21.688021, 11.908962, 10.675203, 9.051885]),
    "Extra Grande":  np.array([299.984274, 156.763059, 84.850866, 76.066647, 64.291297]),
}

# Cores consistentes
cores = {"Pequena": "#9aa0a6", "Média": "#4285f4",
         "Grande": "#ea4335", "Extra Grande": "#34a853"}
marcadores = {"Pequena": "o", "Média": "s", "Grande": "^", "Extra Grande": "D"}

# ============================================================================
# Gráfico 1 — Speedup vs. threads (todas as imagens)
# ============================================================================
fig, ax = plt.subplots(figsize=(7, 5))
p_ideal = np.linspace(1, 16, 100)
ax.plot(p_ideal, p_ideal, 'k--', alpha=0.5, label='Ideal (linear)')

for nome, tp in T_p.items():
    S = T_s[nome] / tp
    ax.plot(P, S, marker=marcadores[nome], color=cores[nome], label=nome, linewidth=2)

ax.set_xlabel("Número de threads ($p$)")
ax.set_ylabel("Speedup $S(p)$")
ax.set_title("Speedup vs. número de threads")
ax.set_xticks(P)
ax.grid(True, alpha=0.3)
ax.legend(loc='upper left')
plt.tight_layout()
plt.savefig("figuras/speedup.png", dpi=200)
plt.close()

# ============================================================================
# Gráfico 2 — Eficiência vs. threads
# ============================================================================
fig, ax = plt.subplots(figsize=(7, 5))
ax.axhline(1.0, color='k', linestyle='--', alpha=0.5, label='Eficiência ideal')

for nome, tp in T_p.items():
    E = (T_s[nome] / tp) / P
    ax.plot(P, E, marker=marcadores[nome], color=cores[nome], label=nome, linewidth=2)

ax.set_xlabel("Número de threads ($p$)")
ax.set_ylabel("Eficiência $E(p)$")
ax.set_title("Eficiência vs. número de threads")
ax.set_xticks(P)
ax.set_ylim(0.4, 1.15)
ax.grid(True, alpha=0.3)
ax.legend(loc='lower left')
plt.tight_layout()
plt.savefig("figuras/eficiencia.png", dpi=200)
plt.close()

# ============================================================================
# Gráfico 3 — Tempo de execução vs. threads (escala log em Y)
# ============================================================================
fig, ax = plt.subplots(figsize=(7, 5))
for nome, tp in T_p.items():
    ax.plot(P, tp, marker=marcadores[nome], color=cores[nome], label=nome, linewidth=2)

ax.set_xlabel("Número de threads ($p$)")
ax.set_ylabel("Tempo de filtragem (s)")
ax.set_title("Tempo de execução vs. número de threads")
ax.set_xticks(P)
ax.set_yscale('log')
ax.grid(True, alpha=0.3, which='both')
ax.legend()
plt.tight_layout()
plt.savefig("figuras/tempos.png", dpi=200)
plt.close()

# ============================================================================
# Gráfico 4 — Escalabilidade: eficiência por tamanho de imagem
# (para discussão de escalabilidade fraca / Gustafson)
# ============================================================================
fig, ax = plt.subplots(figsize=(7, 5))
tamanhos = list(T_p.keys())
for i, p_val in enumerate(P):
    idx = i
    E = [(T_s[n] / T_p[n][idx]) / p_val for n in tamanhos]
    ax.plot(tamanhos, E, marker='o', label=f"$p={p_val}$", linewidth=1.5)

ax.axhline(1.0, color='k', linestyle='--', alpha=0.4)
ax.set_xlabel("Tamanho da imagem")
ax.set_ylabel("Eficiência $E(p)$")
ax.set_title("Eficiência por tamanho de imagem (escalabilidade fraca)")
ax.set_ylim(0.4, 1.15)
ax.grid(True, alpha=0.3)
ax.legend(loc='lower left', ncol=2)
plt.tight_layout()
plt.savefig("figuras/escalabilidade_fraca.png", dpi=200)
plt.close()

print("Gráficos gerados em figuras/:")
for f in ["speedup.png", "eficiencia.png", "tempos.png", "escalabilidade_fraca.png"]:
    print(f"  figuras/{f}")
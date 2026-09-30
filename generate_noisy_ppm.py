import cv2
import numpy as np
import os


def processar_e_converter_p6(caminho_entrada, caminho_saida, densidade_ruido=0.05):
    """
    1. Abre a imagem em tons de cinza (carrega apenas 1 canal para poupar RAM).
    2. Injeta ruído Sal e Pimenta de forma vetorizada.
    3. Converte e grava no formato PPM P6 binário nativo (R=G=B).
    """
    print(f"[*] Carregando {os.path.basename(caminho_entrada)} em Tons de Cinza...")

    # IMREAD_GRAYSCALE carrega a imagem com apenas 1 byte por pixel (1/3 do tamanho RGB)
    img_cinza = cv2.imread(caminho_entrada, cv2.IMREAD_GRAYSCALE)

    if img_cinza is None:
        print(f"[!] Erro: Não foi possível abrir a imagem {caminho_entrada}")
        return

    altura, largura = img_cinza.shape
    max_val = 255
    print(f"    Dimensões detectadas: {largura}x{altura}")

    # 1. Sintetizar randomicamente o ruído Sal e Pimenta (Vetorizado com NumPy)
    print("[*] Injetando ruído sal e pimenta...")
    total_pixels = largura * altura
    num_ruido = int(densidade_ruido * total_pixels)

    # Metade sal (255), metade pimenta (0)
    num_sal = num_ruido // 2
    num_pimenta = num_ruido - num_sal

    # Gera coordenadas lineares únicas para não sobrepor pixels ruidosos no mesmo passo
    indices_ruido = np.random.choice(total_pixels, num_ruido, replace=False)

    # Achata a matriz temporariamente para aplicar o ruído de forma rápida
    img_flat = img_cinza.ravel()
    img_flat[indices_ruido[:num_sal]] = 255  # Sal (Branco)
    img_flat[indices_ruido[num_sal:]] = 0  # Pimenta (Preto)

    # 2. Gravar no formato PPM P6 (Binário RGB)
    print(f"[*] Gravando arquivo PPM P6 em {caminho_saida}...")

    # O cabeçalho do P6 é estritamente texto ASCII terminado em quebra de linha individual (\n)
    header = f"P6\n{largura} {altura}\n{max_val}\n"

    with open(caminho_saida, "wb") as f:
        f.write(header.encode("ascii"))

        # Para otimizar memória em imagens gigantes (como a de 30k), usa-se linha por linha.
        for i in range(altura):
            # Lê a linha atual da imagem em tons de cinza
            linha_cinza = img_cinza[i, :]
            # Replica a linha 3 vezes no eixo de profundidade para virar RGB
            linha_rgb = np.stack([linha_cinza, linha_cinza, linha_cinza], axis=-1)
            f.write(linha_rgb.tobytes())

    print(
        f"[+] Sucesso! Arquivo gerado com {os.path.getsize(caminho_saida) / (1024**2):.2f} MB.\n"
    )


# --- Execução do Pipeline ---
if __name__ == "__main__":
    diretorio_originais = "./data/originais"
    diretorio_saida = "./data"

    # Dicionário mapeando os arquivos originais (.jpg/.png baixados) para os destinos finais .ppm
    imagens_para_processar = {
        # Formato: "nome_arquivo_original.jpg": "nome_saida_no_data.ppm"
        "Bahkauv.jpg": "entrada_media.ppm",
        "Mona_Lisa,_by_Leonardo_da_Vinci,_from_C2RMF.jpg": "entrada_grande.ppm",
        "Karl_Brullov_-_The_Last_Day_of_Pompeii_-_Google_Art_Project.jpg": "entrada_grande_extra.ppm",
    }

    for arquivo_orig, arquivo_ppm in imagens_para_processar.items():
        caminho_in = os.path.join(diretorio_originais, arquivo_orig)
        caminho_out = os.path.join(diretorio_saida, arquivo_ppm)

        if os.path.exists(caminho_in):
            processar_e_converter_p6(
                caminho_in, caminho_out, densidade_ruido=0.05
            )  # 5% de ruído
        else:
            print(
                f"[?] Arquivo {arquivo_orig} não encontrado em '{diretorio_originais}'. Pulando..."
            )

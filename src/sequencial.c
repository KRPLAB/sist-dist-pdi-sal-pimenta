#include "../include/ppm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função auxiliar para trocar dois valores de unsigned char
static void trocar(unsigned char *a, unsigned char *b) {
	unsigned char temp = *a;
	*a = *b;
	*b = temp;
}

// Ordenação simples por seleção (Selection Sort) para 9 elementos
static void ordenar_janela(unsigned char vetor[9]) {
	for (int i = 0; i < 8; i++) {
		int min_idx = i;
		for (int j = i + 1; j < 9; j++) {
			if (vetor[j] < vetor[min_idx]) {
				min_idx = j;
			}
		}
		if (min_idx != i) {
			trocar(&vetor[i], &vetor[min_idx]);
		}
	}
}

// Aloca memória para uma nova imagem com as mesmas dimensões
struct imagem_ppm *criar_imagem_ppm(int largura, int altura, int max_cor) {
	struct imagem_ppm *img = malloc(sizeof(struct imagem_ppm));
	if (!img)
		return NULL;

	img->largura = largura;
	img->altura = altura;
	img->max_cor = max_cor;

	size_t tamanho_dados = (size_t)largura * altura * 3;
	img->dados = malloc(tamanho_dados);
	if (!img->dados) {
		free(img);
		return NULL;
	}

	return img;
}

struct imagem_ppm *ler_ppm(const char *caminho) {
	FILE *fp = fopen(caminho, "rb");
	if (!fp) {
		fprintf(stderr, "Erro ao abrir o arquivo de entrada: %s\n", caminho);
		return NULL;
	}

	char formato[3];
	if (!(fscanf(fp, "%2s", formato)) || strcmp(formato, "P6") != 0) {
		fprintf(stderr, "Formato invalido (deve ser P6): %s\n", caminho);
		fclose(fp);
		return NULL;
	}

	int c = fgetc(fp);
	while (c == '#' || c == '\n' || c == '\r' || c == ' ') {
		if (c == '#') {
			while ((c = fgetc(fp)) != '\n' && c != EOF)
				;
		} else {
			c = fgetc(fp);
		}
	}
	ungetc(c, fp);

	struct imagem_ppm *img = malloc(sizeof(struct imagem_ppm));
	if (!img) {
		fclose(fp);
		return NULL;
	}

	if (fscanf(fp, "%d %d %d", &img->largura, &img->altura, &img->max_cor) !=
	    3) {
		fprintf(stderr, "Erro ao ler o cabecalho PPM.\n");
		fclose(fp);
		free(img);
		return NULL;
	}
	fgetc(fp); // Consumir o \n do cabeçalho

	size_t tamanho_dados = (size_t)img->largura * img->altura * 3;
	img->dados = malloc(tamanho_dados);

	size_t lidos = fread(img->dados, 1, tamanho_dados, fp);
	if (lidos != tamanho_dados) {
		fprintf(stderr, "Aviso: Leitura incompleta dos bytes de pixel.\n");
	}

	fclose(fp);
	return img;
}

int salvar_ppm(const char *caminho, const struct imagem_ppm *img) {
	FILE *fp = fopen(caminho, "wb");
	if (!fp) {
		fprintf(stderr, "Erro ao abrir o arquivo para escrita: %s\n", caminho);
		return 0;
	}

	fprintf(fp, "P6\n%d %d\n%d\n", img->largura, img->altura, img->max_cor);
	size_t tamanho_dados = (size_t)img->largura * img->altura * 3;
	fwrite(img->dados, 1, tamanho_dados, fp);

	fclose(fp);
	return 1;
}

void liberar_ppm(struct imagem_ppm *img) {
	if (img) {
		if (img->dados)
			free(img->dados);
		free(img);
	}
}

// Aplica o filtro da mediana 3x3 na imagem de entrada
struct imagem_ppm *aplicar_filtro_mediana_3x3(const struct imagem_ppm *img_in) {
	int larg = img_in->largura;
	int alt = img_in->altura;

	struct imagem_ppm *img_out = criar_imagem_ppm(larg, alt, img_in->max_cor);
	if (!img_out)
		return NULL;

	// Copiar pixels da borda (linhas e colunas extremas) sem alterar
	memcpy(img_out->dados, img_in->dados, (size_t)larg * alt * 3);

	// Percorrer pixels internos (ignorando borda de 1 pixel)
	for (int y = 1; y < alt - 1; y++) {
		for (int x = 1; x < larg - 1; x++) {

			// Filtro para cada canal de cor (0=R, 1=G, 2=B)
			for (int canal = 0; canal < 3; canal++) {
				unsigned char janela[9];
				int idx = 0;

				// Coletar a vizinhança 3x3
				for (int dy = -1; dy <= 1; dy++) {
					for (int dx = -1; dx <= 1; dx++) {
						int px = x + dx;
						int py = y + dy;
						janela[idx++] =
						    img_in->dados[(py * larg + px) * 3 + canal];
					}
				}

				// Ordenar e extrair a mediana (elemento do meio: índice 4)
				ordenar_janela(janela);
				img_out->dados[(y * larg + x) * 3 + canal] = janela[4];
			}
		}
	}

	return img_out;
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("Uso: %s <entrada.ppm> <saida.ppm>\n", argv[0]);
		return 1;
	}

	const char *arquivo_entrada = argv[1];
	const char *arquivo_saida = argv[2];

	printf("Lendo imagem: %s...\n", arquivo_entrada);
	struct imagem_ppm *img_in = ler_ppm(arquivo_entrada);
	if (!img_in)
		return 1;

	printf("Imagem lida (%dx%d). Aplicando Filtro da Mediana 3x3...\n",
	       img_in->largura, img_in->altura);

	struct imagem_ppm *img_out = aplicar_filtro_mediana_3x3(img_in);
	if (!img_out) {
		liberar_ppm(img_in);
		return 1;
	}

	printf("Salvando resultado em: %s...\n", arquivo_saida);
	if (salvar_ppm(arquivo_saida, img_out)) {
		printf("Filtro aplicado com sucesso!\n");
	}

	liberar_ppm(img_in);
	liberar_ppm(img_out);
	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para armazenar a imagem na memória
struct imagem_ppm {
	int largura;
	int altura;
	int max_cor;
	unsigned char *dados; // Array contendo os bytes R, G, B em sequência
};

// Função para ler arquivo PPM no formato P6
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

	// Ignorar comentarios iniciados por '#'
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

	if (fscanf(fp, "%d %d %d", &img->largura, &img->altura, &img->max_cor) !=
	    3) {
		fprintf(stderr, "Erro ao ler o cabecalho PPM.\n");
		fclose(fp);
		free(img);
		return NULL;
	}
	fgetc(fp); // Consumir o caractere de nova linha apos o cabecalho

	size_t tamanho_dados = (size_t)img->largura * img->altura * 3;
	img->dados = (unsigned char *)malloc(tamanho_dados);

	size_t lidos = fread(img->dados, 1, tamanho_dados, fp);
	if (lidos != tamanho_dados) {
		fprintf(stderr, "Aviso: Leitura incompleta dos bytes de pixel.\n");
	}

	fclose(fp);
	return img;
}

// Função para escrever arquivo PPM no formato P6
int salvar_ppm(const char *caminho, const struct imagem_ppm *img) {
	FILE *fp = fopen(caminho, "wb");
	if (!fp) {
		fprintf(stderr, "Erro ao abrir o arquivo para escrita: %s\n", caminho);
		return 0;
	}

	// Escreve o cabeçalho P6
	fprintf(fp, "P6\n%d %d\n%d\n", img->largura, img->altura, img->max_cor);

	// Escreve os dados binarios dos pixels
	size_t tamanho_dados = (size_t)img->largura * img->altura * 3;
	fwrite(img->dados, 1, tamanho_dados, fp);

	fclose(fp);
	return 1;
}

// Libera a memória alocada para a imagem
void liberar_ppm(struct imagem_ppm *img) {
	if (img) {
		if (img->dados)
			free(img->dados);
		free(img);
	}
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("Uso: %s <entrada.ppm> <saida.ppm>\n", argv[0]);
		return 1;
	}

	const char *arquivo_entrada = argv[1];
	const char *arquivo_saida = argv[2];

	printf("Lendo imagem: %s...\n", arquivo_entrada);
	struct imagem_ppm *img = ler_ppm(arquivo_entrada);
	if (!img)
		return 1;

	printf("Imagem lida com sucesso: %dx%d pixels, cor max: %d\n", img->largura,
	       img->altura, img->max_cor);

	// Teste simples de manipulacao: converter para tons de cinza
	for (int i = 0; i < img->largura * img->altura; i++) {
		unsigned char r = img->dados[i * 3 + 0];
		unsigned char g = img->dados[i * 3 + 1];
		unsigned char b = img->dados[i * 3 + 2];

		// Média ponderada para tom de cinza (luminância)
		unsigned char cinza =
		    (unsigned char)(0.299 * r + 0.587 * g + 0.114 * b);

		img->dados[i * 3 + 0] = cinza;
		img->dados[i * 3 + 1] = cinza;
		img->dados[i * 3 + 2] = cinza;
	}

	printf("Salvando imagem modificada em: %s...\n", arquivo_saida);
	if (salvar_ppm(arquivo_saida, img)) {
		printf("Processamento concluido com sucesso!\n");
	}

	liberar_ppm(img);
	return 0;
}

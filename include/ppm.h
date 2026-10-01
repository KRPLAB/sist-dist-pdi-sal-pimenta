#ifndef PPM_H
#define PPM_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Estrutura para armazenar a imagem PPM na memória.
 */
struct imagem_ppm {
    int largura;
    int altura;
    int max_cor;
    unsigned char *dados;
};

/**
 * @brief Lê uma imagem PPM de um arquivo.
 * @param caminho Caminho do arquivo PPM.
 * @return Ponteiro para a estrutura da imagem PPM, ou NULL em caso de erro.
 */
struct imagem_ppm *ler_ppm(const char *caminho);

/**
 * @brief Salva uma imagem PPM em um arquivo.
 * @param caminho Caminho do arquivo PPM.
 * @param img Ponteiro para a estrutura da imagem PPM.
 * @return 1 em caso de sucesso, ou 0 em caso de erro.
 */
int salvar_ppm(const char *caminho, const struct imagem_ppm *img);

/**
 * @brief Libera a memória alocada para uma imagem PPM.
 * @param img Ponteiro para a estrutura da imagem PPM.
 */
void liberar_ppm(struct imagem_ppm *img);

/**
 * @brief Cria uma nova imagem PPM.
 * @param largura Largura da imagem.
 * @param altura Altura da imagem.
 * @param max_cor Valor máximo de cor.
 * @return Ponteiro para a estrutura da imagem PPM, ou NULL em caso de erro.
 */
struct imagem_ppm *criar_imagem_ppm(int largura, int altura, int max_cor);

/**
 * @brief Aplica o filtro da mediana 5x5 a uma imagem PPM.
 * @param img_in Imagem de entrada.
 * @param tempo_saida Ponteiro que recebe o tempo de processamento do filtro,
 *                    ou NULL para ignorar a medição.
 * @return Ponteiro para a imagem filtrada, ou NULL em caso de erro.
 */
struct imagem_ppm *aplicar_filtro_mediana_5x5(const struct imagem_ppm *img_in,
                                              double *tempo_saida);

#endif // PPM_H
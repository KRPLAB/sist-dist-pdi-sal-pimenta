# BCC - Projeto de Sistemas Distribuídos (2026/2)
## Trabalho 1: Remoção de Ruído em Imagens com OpenMP

Este repositório contém a implementação prática de um algoritmo de filtragem espacial para remoção de ruído do tipo **Sal e Pimenta (Impulse Noise)** em imagens digitais. O projeto contempla uma versão puramente sequencial e uma versão paralelizada utilizando a biblioteca **OpenMP** para arquiteturas de memória compartilhada.

### Integrantes do Grupo
* **Kauan da Rosa Paulino** - 20251pin10020017@estudantes.ifpr.edu.br / 20251pin10020017

---

### Conjunto de Dados de Teste

Como os arquivos de imagem de alta resolução possuem tamanho elevado para versionamento direto via Git, a tabela a seguir lista as fontes originais e dimensões das imagens utilizadas nos testes:

| Tamanho | Dimensões | Nome do Arquivo Local | Fonte Original / Descrição |
| :--- | :--- | :--- | :--- |
| **Pequena** | $512 \times 512$ | `entrada_pequena.ppm` | [Kaggle - Salt and Pepper Noise Dataset](https://www.kaggle.com/datasets/rajneesh231/salt-and-pepper-noise-images?select=Noisy_folder) (`noisy_10961455225_0786d3edd2_c.jpg`) |
| **Média** | $3648 \times 3648$ | `entrada_media.ppm` | [Wikimedia Commons - Bahkauv](https://commons.wikimedia.org/wiki/File:Bahkauv.jpg) |
| **Grande** | $7854 \times 11498$ | `entrada_grande.ppm` | [Wikimedia Commons - Mona Lisa (C2RMF)](https://commons.wikimedia.org/wiki/File:Mona_Lisa,_by_Leonardo_da_Vinci,_from_C2RMF.jpg) |
| **Extra Grande** | $30000 \times 21059$ | `entrada_grande_extra.ppm` | [Wikimedia Commons - The Last Day of Pompeii (Google Art Project)](https://commons.wikimedia.org/wiki/File:Karl_Brullov_-_The_Last_Day_of_Pompeii_-_Google_Art_Project.jpg) |

---

### Estrutura de Pastas do Projeto

```text
.
├── data/                  # Imagens convertidas para o formato binário PPM (P6)
│   ├── entrada_pequena.ppm
│   ├── entrada_media.ppm
│   ├── entrada_grande.ppm
│   ├── entrada_grande_extra.ppm
│   └── originais/         # Arquivos originais (.jpg / .png)
├── include/
│   └── ppm.h              # Cabeçalho com estruturas de dados e protótipos de funções
├── src/
│   ├── sequencial.c       # Algoritmo de filtragem sequencial
│   └── paralelo.c         # Algoritmo paralelizado com OpenMP
├── generate_noisy_ppm.py  # Script em Python para síntese de ruído e conversão PPM P6
├── run_tests.sh           # Script em Bash para automação dos testes de benchmark
├── referencias.bib        # Referências bibliográficas BibTeX
├── relatorio.tex          # Relatório técnico em LaTeX
└── README.md

```
---

### Requisitos de Compilação e Execução

O projeto foi desenvolvido em ambiente Linux e requer o compilador `gcc` com suporte nativo ao OpenMP.

#### 1. Compilar a versão Sequencial
```bash
gcc -O3 src/sequencial.c -o sequencial
```

#### 2. Compilar a versão Paralela (com OpenMP)
```bash
gcc -O3 -fopenmp src/paraleo.c -o paralelo
```
*Nota: A flag `-O3` ativa otimizações agressivas de laço pelo compilador, garantindo um cenário de benchmark real.*

#### 3. Execução Manual
Para rodar os programas individualmente, passe os parâmetros de arquivo e tamanho da janela k × k (ex: janela 3x3):
```bash
./sequencial data/entrada_media.ppm data/saida_sequencial.ppm 3
./paralelo data/entrada_media.ppm data/saida_paralela.ppm 3
```

#### 4. Execução Automática dos Testes
Para rodar a bateria de testes de desempenho (5 execuções por configuração para gerar as médias de 1, 2, 4 e 8 threads), basta dar permissão e rodar o script automatizado:
```bash
chmod +x run_tests.sh
./run_tests.sh
```

---

### Referências Utilizadas

As referências abaixo foram integradas ao arquivo `referencias.bib` e mapeadas no relatório técnico do projeto:

```bibtex
@book{vansteen2023distributed,
  author    = {van Steen, Maarten and Tanenbaum, Andrew S.},
  title     = {Distributed Systems},
  edition   = {4th},
  year      = {2023},
  publisher = {Maarten van Steen},
  isbn      = {978-90-815406-3-6},
  url       = {https://distributed-systems.net}
}

@book{eijkhout2022arthpc1,
  author    = {Eijkhout, Victor and Chow, Edmond and van de Geijn, Robert},
  title     = {The Art of HPC, Volume 1: The Science of Computing},
  edition   = {3rd},
  year      = {2022},
  publisher = {Lulu.com},
  note      = {Disponível em: \url{https://theartofhpc.com/}},
  isbn      = {9781387388561}
}

@book{eijkhout2022arthpc2,
  author    = {Eijkhout, Victor},
  title     = {The Art of HPC, Volume 2: Parallel Programming for Science and Engineering},
  year      = {2022},
  publisher = {Lulu.com},
  note      = {Disponível em: \url{https://theartofhpc.com/}}
}

@misc{candido2026sisdis,
  author       = {Candido, Gabriel Vinicius Canzi},
  title        = {Material complementar de {Projeto de Sistemas Distribuídos} --- {BCC}},
  howpublished = {Site do curso, \url{https://gvcc.dev.br/teaching/bcc-sisdis/}},
  year         = {2026},
  institution  = {Instituto Federal do Paraná (IFPR) - Campus Pinhais},
  note         = {Notas de aula e exemplos em OpenMP. Acesso em: 25 set. 2026. Última modificação: 17 set. 2026}
}

@book{gonzalez2010processamento,
  author    = {Gonzalez, Rafael C. and Woods, Richard E.},
  title     = {Processamento Digital de Imagens},
  edition   = {3rd},
  year      = {2010},
  publisher = {Pearson Prentice Hall},
  address   = {S{\~a}o Paulo},
  isbn      = {978-85-7605-401-6}
}

@misc{burkardt2017imagedenoise,
  author       = {Burkardt, John},
  title        = {{IMAGE\_DENOISE\_OPENMP} --- Remove Noise from an Image using {OpenMP}},
  howpublished = {\url{https://people.math.sc.edu/Burkardt/c_src/image_denoise_openmp/image_denoise_openmp.html}},
  year         = {2017},
  note         = {Department of Mathematics, University of South Carolina. Acesso em: 29 set. 2026}
}

@misc{rajneesh231_salt_pepper_2024,
  author       = {Rajneesh, Kumar},
  title        = {Salt and Pepper Noise Dataset: Clean vs Noisy Image},
  year         = {2024},
  publisher    = {Kaggle},
  journal      = {Kaggle Datasets},
  howpublished = {\url{https://www.kaggle.com/datasets/rajneesh231/salt-and-pepper-noise-images}},
  note         = {Acessado em: 29 de setembro de 2026}
}

```

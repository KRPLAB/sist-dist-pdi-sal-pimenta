# BCC - Projeto de Sistemas Distribuídos (2026/2)
## Trabalho 1: Remoção de Ruído em Imagens com OpenMP

Este repositório contém a implementação prática de um algoritmo de filtragem espacial para remoção de ruído do tipo **Sal e Pimenta (Impulse Noise)** em imagens digitais. O projeto contempla uma versão puramente sequencial e uma versão paralelizada utilizando a biblioteca **OpenMP** para arquiteturas de memória compartilhada.

O filtro implementado é o **filtro da mediana com janela 5×5**, aplicado independentemente a cada canal de cor (R, G, B). As bordas da imagem (duas linhas e duas colunas externas) são preservadas inalteradas.

### Integrante
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

Todas as imagens foram convertidas para o formato binário **PPM P6** e receberam ruído sintético do tipo sal e pimenta por meio do script `generate_noisy_ppm.py` (exceto a de tamanho pequeno, que já foi obtida com ruído).

---

### Estrutura de Pastas do Projeto

```text
.
├── data/                  # Imagens convertidas para o formato binário PPM (P6), não versionados no Git por serem muito grandes
│   ├── entrada_pequena.ppm
│   ├── entrada_media.ppm
│   ├── entrada_grande.ppm
│   ├── entrada_grande_extra.ppm
│   ├── logs/              # Logs de benchmark (gerados por run_tests.sh)
│   └── originais/         # Arquivos originais (.jpg / .png), não versionados no Git por serem muito grandes
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

### Reproduzindo os Testes

As imagens de entrada (`.ppm`) **não estão versionadas** neste repositório por excederem 3 GB no total. Para reproduzir os experimentos:

1. Baixe os arquivos originais a partir dos links da tabela acima.
2. Coloque-os em `data/originais/`.
3. Rode o script de conversão, que aplica ruído sal e pimenta sintético e
   converte para PPM P6:
   ```bash
   python3 generate_noisy_ppm.py
   ```
4. Os arquivos `data/entrada_*.ppm` serão gerados.
5. Compile os binários (seção anterior) e rode:
   ```bash
   ./run_tests.sh
   ```
---

### Requisitos de Compilação e Execução

O projeto foi desenvolvido em ambiente Linux e requer o compilador `gcc` com suporte nativo ao OpenMP.

#### 1. Compilar a versão Sequencial
```bash
gcc -std=c11 -Wall -Wextra -O3 -fopenmp src/sequencial.c -o sequencial  
```
*Nota: a versão sequencial também é compilada com `-fopenmp` neste projeto, pois utiliza `omp_get_wtime()` para medição de tempo. O uso de OpenMP limita-se à instrumentação, sem paralelismo efetivo.*


#### 2. Compilar a versão Paralela (com OpenMP)
```bash
gcc -std=c11 -Wall -Wextra -O3 -fopenmp src/paralelo.c -o paralelo
```
*Nota: `-O3` é obrigatório nas duas versões para uma comparação justa. Compilar o sequencial sem otimização e o paralelo com `-O3` infla artificialmente o speedup.*

#### 3. Execução Manual

Ambos os programas aceitam dois argumentos posicionais: **arquivo de entrada** e **arquivo de saída**, ambos em formato PPM P6.

```bash
./sequencial data/entrada_media.ppm data/saida_sequencial.ppm
./paralelo  data/entrada_media.ppm data/saida_paralela.ppm
```

O tamanho da janela do filtro está fixado em **5×5** por decisão de projeto (ver relatório técnico). A região cronometrada pelo programa corresponde exclusivamente ao laço de filtragem; leitura, alocação e escrita de arquivo são excluídas das medições.

**Controle do número de threads (versão paralela):** utilize a variável de ambiente `OMP_NUM_THREADS`. Isso permite reutilizar o mesmo binário para toda a bateria de testes.

```bash
OMP_NUM_THREADS=4 ./paralelo data/entrada_grande.ppm data/saida_4t.ppm
OMP_NUM_THREADS=8 ./paralelo data/entrada_grande.ppm data/saida_8t.ppm
```

#### 4. Execução Automática dos Testes

Para rodar a bateria completa de benchmark (5 execuções por configuração), execute o script:

```bash
chmod +x run_tests.sh
./run_tests.sh
```

O script gera logs em `data/logs/` com os tempos internos (medidos por `omp_get_wtime()`) e os tempos totais reportados por `/usr/bin/time`.

**Recomendações antes da bateria:**

```bash
# Fixar governor da CPU em performance (evita throttling adaptativo)
echo performance | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor
```

*Reverter ao padrão após os testes:*
```bash
echo schedutil | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor
```

---

### Métricas de Desempenho

As métricas reportadas no relatório foram calculadas a partir de:

- **$T_s$**: tempo médio do filtro sequencial (5 execuções).
- **$T_p(p)$**: tempo médio do filtro paralelo com $p$ threads (5 execuções).
- **$S(p) = T_s / T_p(p)$**: *speedup*.
- **$E(p) = S(p) / p$**: eficiência.
- **$T_o(p) = T_p(p) \cdot p - T_s$**: *overhead*.

A fração sequencial efetiva foi estimada pela diferença entre o tempo total do programa (`time` do Linux) e o tempo do laço de filtragem (`omp_get_wtime()`), sendo utilizada como parâmetro $\beta$ na Lei de Amdahl.

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

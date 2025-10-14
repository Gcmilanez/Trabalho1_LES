import re
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# A sua função parse_graph_data pode continuar a mesma...
def parse_graph_data(file_content):
    matrix_pattern = re.compile(r"Grafo - Matriz Adjacência:([\s\S]*?)Grafo Lista de Adjacência:")
    list_pattern = re.compile(r"Grafo Lista de Adjacência:([\s\S]*?)Grafo - Hash Map:")
    hash_pattern = re.compile(r"Grafo - Hash Map:([\s\S]*)")

    matrix_data = matrix_pattern.findall(file_content)
    list_data = list_pattern.findall(file_content)
    hash_data = hash_pattern.findall(file_content)

    def extract_graph_metrics(data, algorithm_name):
        metrics = []
        dataset_pattern = re.compile(r"Arquivo: ([\w\.-]+\.txt)")
        construction_time_pattern = re.compile(r"Tempo construção \(ms\): ([\d\.]+)")
        bfs_time_pattern = re.compile(r"Tempo 32x BFS \(ms\): ([\d\.]+)")
        cache_misses_pattern = re.compile(r"cache-misses\s*#\s*([\d\.]+,[\d]+)\s*%")
        l1_misses_pattern = re.compile(r"L1-dcache-load-misses\s*#\s*([\d\.]+,[\d]+)\s*%")
        l2_accesses_pattern = re.compile(r"([\d\.,]+)\s*l2_cache_accesses_from_dc_misses")
        l2_misses_pattern = re.compile(r"([\d\.,]+)\s*l2_cache_misses_from_dc_misses")
        branch_misses_pattern = re.compile(r"branch-misses\s*#\s*([\d\.]+,[\d]+)\s*%")
        ipc_pattern = re.compile(r"instructions\s*#\s*([\d\.]+,[\d]+)\s*insn per cycle")
        
        for block in re.split(r"perf stat", data):
            if not block.strip():
                continue

            dataset_match = dataset_pattern.search(block)
            if not dataset_match:
                continue
                
            construction_time_match = construction_time_pattern.search(block)
            bfs_time_match = bfs_time_pattern.search(block)
            cache_misses_match = cache_misses_pattern.search(block)
            l1_misses_match = l1_misses_pattern.search(block)
            l2_accesses_match = l2_accesses_pattern.search(block)
            l2_misses_match = l2_misses_pattern.search(block)
            branch_misses_match = branch_misses_pattern.search(block)
            ipc_match = ipc_pattern.search(block)
            
            l2_miss_perc = np.nan
            if l2_accesses_match and l2_misses_match:
                try:
                    l2_accesses = float(l2_accesses_match.group(1).replace('.', '').replace(',', '.'))
                    l2_misses = float(l2_misses_match.group(1).replace('.', '').replace(',', '.'))
                    if l2_accesses > 0:
                        l2_miss_perc = (l2_misses / l2_accesses) * 100
                except (ValueError, IndexError):
                    pass

            metrics.append({
                "Algorithm": algorithm_name,
                "Dataset": dataset_match.group(1),
                "Construction Time (ms)": float(construction_time_match.group(1)) if construction_time_match else np.nan,
                "32x BFS Time (ms)": float(bfs_time_match.group(1)) if bfs_time_match else np.nan,
                "Cache Misses (%)": float(cache_misses_match.group(1).replace(",", ".")) if cache_misses_match else np.nan,
                "L1 dcache-load-misses (%)": float(l1_misses_match.group(1).replace(",", ".")) if l1_misses_match else np.nan,
                "L2 Cache Misses (%)": l2_miss_perc,
                "Branch Misses (%)": float(branch_misses_match.group(1).replace(",", ".")) if branch_misses_match else np.nan,
                "Instructions per Cycle": float(ipc_match.group(1).replace(",", ".")) if ipc_match else np.nan
            })
        return metrics

    all_metrics = []
    if matrix_data:
        all_metrics.extend(extract_graph_metrics(matrix_data[0], "Matriz de Adjacência"))
    if list_data:
        all_metrics.extend(extract_graph_metrics(list_data[0], "Lista de Adjacência"))
    if hash_data:
        all_metrics.extend(extract_graph_metrics(hash_data[0], "Hash Map"))

    return pd.DataFrame(all_metrics)

# *** FUNÇÃO CORRIGIDA ABAIXO ***
def plot_graph_metrics(df):
    metrics_to_plot = [
        "Construction Time (ms)", "32x BFS Time (ms)", "Cache Misses (%)",
        "L1 dcache-load-misses (%)", "L2 Cache Misses (%)", "Branch Misses (%)", "Instructions per Cycle"
    ]
    
    for metric in metrics_to_plot:
        if df[metric].isnull().all():
            print(f"Aviso: Pulando a métrica '{metric}' pois não há dados para plotar.")
            continue

        plt.style.use('seaborn-v0_8-whitegrid')
        fig, ax = plt.subplots(figsize=(12, 7))
        
        # *** ALTERAÇÃO AQUI: troquei pivot por pivot_table ***
        # Isso lida com entradas duplicadas tirando a média dos valores.
        pivot_df = df.pivot_table(index='Dataset', columns='Algorithm', values=metric, aggfunc='mean')
        
        pivot_df.plot(kind='bar', ax=ax, width=0.8)
        
        ax.set_xlabel("Dataset", fontsize=12)
        ax.set_ylabel(metric, fontsize=12)
        ax.set_title(f"Comparação de {metric} para Algoritmos de Grafo", fontsize=14)
        plt.xticks(rotation=45, ha="right")
        ax.legend(title="Algoritmo", loc='upper left')

        if "Time" in metric:
            fig.canvas.draw()
            ymin, _ = ax.get_ylim()
            if ymin > 0:
                # ax.set_yscale('log') # Descomente se quiser usar escala log
                pass
            else:
                print(f"Aviso: '{metric}' contém ou se aproxima de valores não positivos. Usando escala linear.")
        
        plt.tight_layout()
        file_name = f"graph_{metric.replace(' ', '_').replace('(', '').replace(')', '').replace('%', 'perc')}.png"
        plt.savefig(file_name)
        plt.close(fig)

# O resto do seu código para ler o arquivo e chamar a função
try:
    with open("Resultados.txt", "r", encoding="utf-8") as f:
        file_content = f.read()
    df_graphs = parse_graph_data(file_content)
    plot_graph_metrics(df_graphs)
    print("Gráficos dos grafos gerados com sucesso!")
except FileNotFoundError:
    print("Erro: Arquivo 'Resultados.txt' não encontrado.")
except Exception as e:
    print(f"Ocorreu um erro inesperado: {e}")
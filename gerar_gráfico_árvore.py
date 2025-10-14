import re
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def parse_tree_data(file_content):
    bst_recursive_pattern = re.compile(r"BST - Recursiva:([\s\S]*?)BST - if-then-else:")
    bst_iterative_pattern = re.compile(r"BST - if-then-else:([\s\S]*?)BST - AVL:")
    avl_pattern = re.compile(r"BST - AVL:([\s\S]*?)Grafo - Matriz Adjacência:")

    bst_recursive_data = bst_recursive_pattern.findall(file_content)
    bst_iterative_data = bst_iterative_pattern.findall(file_content)
    avl_data = avl_pattern.findall(file_content)

    def extract_metrics(data, algorithm_name):
        metrics = []
        # Padrões Regex para extração
        searches_pattern = re.compile(r"buscas: (\d+)")
        insertion_time_pattern = re.compile(r"Tempo inserção \(ms\): ([\d\.]+)")
        search_time_pattern = re.compile(r"Tempo busca    \(ms\): ([\d\.]+)")
        cache_misses_pattern = re.compile(r"cache-misses\s*#\s*([\d\.]+,[\d]+)\s*%")
        l1_misses_pattern = re.compile(r"L1-dcache-load-misses\s*#\s*([\d\.]+,[\d]+)\s*%")
        l2_accesses_pattern = re.compile(r"([\d\.,]+)\s*l2_cache_accesses_from_dc_misses")
        l2_misses_pattern = re.compile(r"([\d\.,]+)\s*l2_cache_misses_from_dc_misses")
        branch_misses_pattern = re.compile(r"branch-misses\s*#\s*([\d\.]+,[\d]+)\s*%")
        ipc_pattern = re.compile(r"instructions\s*#\s*([\d\.]+,[\d]+)\s*insn per cycle")

        for block in re.split(r"perf stat", data):
            if not block.strip():
                continue
            
            searches_match = searches_pattern.search(block)
            if not searches_match:
                continue

            # Extrai os valores
            insertion_time_match = insertion_time_pattern.search(block)
            search_time_match = search_time_pattern.search(block)
            cache_misses_match = cache_misses_pattern.search(block)
            l1_misses_match = l1_misses_pattern.search(block)
            l2_accesses_match = l2_accesses_pattern.search(block)
            l2_misses_match = l2_misses_pattern.search(block)
            branch_misses_match = branch_misses_pattern.search(block)
            ipc_match = ipc_pattern.search(block)

            # Calcula L2 Misses (%) se os dados estiverem disponíveis
            l2_miss_perc = np.nan
            if l2_accesses_match and l2_misses_match:
                try:
                    l2_accesses = float(l2_accesses_match.group(1).replace('.', '').replace(',', '.'))
                    l2_misses = float(l2_misses_match.group(1).replace('.', '').replace(',', '.'))
                    if l2_accesses > 0:
                        l2_miss_perc = (l2_misses / l2_accesses) * 100
                except (ValueError, IndexError):
                    pass # Deixa como NaN se a conversão falhar

            metrics.append({
                "Algorithm": algorithm_name,
                "Searches": int(searches_match.group(1)),
                "Insertion Time (ms)": float(insertion_time_match.group(1)) if insertion_time_match else np.nan,
                "Search Time (ms)": float(search_time_match.group(1)) if search_time_match else np.nan,
                "Cache Misses (%)": float(cache_misses_match.group(1).replace(",", ".")) if cache_misses_match else np.nan,
                "L1 dcache-load-misses (%)": float(l1_misses_match.group(1).replace(",", ".")) if l1_misses_match else np.nan,
                "L2 Cache Misses (%)": l2_miss_perc,
                "Branch Misses (%)": float(branch_misses_match.group(1).replace(",", ".")) if branch_misses_match else np.nan,
                "Instructions per Cycle": float(ipc_match.group(1).replace(",", ".")) if ipc_match else np.nan
            })
        return metrics
    
    all_metrics = []
    if bst_recursive_data:
        all_metrics.extend(extract_metrics(bst_recursive_data[0], "BST Recursiva"))
    if bst_iterative_data:
        all_metrics.extend(extract_metrics(bst_iterative_data[0], "BST if-then-else"))
    if avl_data:
        all_metrics.extend(extract_metrics(avl_data[0], "AVL"))

    return pd.DataFrame(all_metrics)

def plot_tree_metrics(df):
    metrics_to_plot = [
        "Insertion Time (ms)", "Search Time (ms)", "Cache Misses (%)",
        "L1 dcache-load-misses (%)", "L2 Cache Misses (%)", "Branch Misses (%)", "Instructions per Cycle"
    ]
    
    for metric in metrics_to_plot:
        if df[metric].isnull().all():
            print(f"Aviso: Pulando a métrica '{metric}' pois não há dados para plotar.")
            continue
            
        plt.style.use('seaborn-v0_8-whitegrid')
        fig, ax = plt.subplots(figsize=(10, 6))
        
        for name, group in df.groupby('Algorithm'):
            ax.plot(group['Searches'], group[metric], marker='o', linestyle='-', label=name)
        
        ax.set_xlabel("Número de Buscas", fontsize=12)
        ax.set_ylabel(metric, fontsize=12)
        ax.set_title(f"Comparação de {metric} para Algoritmos de Árvore", fontsize=14)
        ax.legend(title="Algoritmo", loc='upper left')
        ax.set_xscale('log')

        if "Time" in metric:
            fig.canvas.draw()
            ymin, _ = ax.get_ylim()
            if ymin > 0:
                ax.set_yscale('log')
            else:
                print(f"Aviso: '{metric}' contém ou se aproxima de valores não positivos. Usando escala linear.")

        plt.tight_layout()
        file_name = f"tree_{metric.replace(' ', '_').replace('(', '').replace(')', '').replace('%', 'perc')}.png"
        plt.savefig(file_name)
        plt.close(fig)

# Carregue o conteúdo do seu arquivo aqui
try:
    with open("Resultados.txt", "r", encoding="utf-8") as f:
        file_content = f.read()
    df_trees = parse_tree_data(file_content)
    plot_tree_metrics(df_trees)
    print("Gráficos das árvores (com L2) gerados com sucesso!")
except FileNotFoundError:
    print("Erro: Arquivo 'Resultados.txt' não encontrado.")
except Exception as e:
    print(f"Ocorreu um erro inesperado: {e}")
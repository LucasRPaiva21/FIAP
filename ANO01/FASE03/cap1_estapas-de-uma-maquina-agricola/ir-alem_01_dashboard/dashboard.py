import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Cores/visual
sns.set_style("whitegrid")

plt.rcParams["figure.figsize"] = (10, 5)
plt.rcParams["font.size"] = 11

ROSA = "#E78AC3"
ROSA_ESCURO = "#D65F9E"
ROSA_CLARO = "#F6D4E6"
AMARELO = "#F9E79F"
AMARELO_OURO = "#F4D03F"

# leitura csv
df = pd.read_csv("dados_esp32 (1).csv", sep=";")

# Limpar nomes das colunas
df.columns = df.columns.str.strip().str.upper()

# Remover colunas totalmente vazias e linhas vazias
df = df.dropna(axis=1, how="all")
df = df.dropna(how="all") #só para evitar que o código 'quebre'

# Converter colunas numéricas
colunas_numericas = [
    "TEMPO_MS",
    "NITROGENIO",
    "FOSFORO",
    "POTASSIO",
    "LDR",
    "PH",
    "UMIDADE",
    "TEMPERATURA",
    "PREVISAO_CHUVA",
    "BOMBA"
]

for col in colunas_numericas:
    if col in df.columns:
        df[col] = pd.to_numeric(df[col], errors="coerce")

# validação das colunas
colunas_necessarias = [
    "TEMPO_MS",
    "NITROGENIO",
    "FOSFORO",
    "POTASSIO",
    "PH",
    "UMIDADE",
    "TEMPERATURA",
    "PREVISAO_CHUVA",
    "BOMBA"
]

for col in colunas_necessarias:
    if col not in df.columns:
        raise ValueError(f"Coluna ausente no CSV: {col}")

df = df.dropna(subset=colunas_necessarias)

# =========================
# RESUMO NO TERMINAL
# =========================
print("\n𐙚⋆.˚RESUMO ")
print("------------------------")
print(f"Umidade atual: {df['UMIDADE'].iloc[-1]:.2f}%")
print(f"Temperatura média: {df['TEMPERATURA'].mean():.1f}°C")
print(f"pH médio: {df['PH'].mean():.2f}")
print(f"Irrigação ligada: {(df['BOMBA'] == 1).sum()} registros")
print("------------------------")

# UMIDADE ----------------------------------
plt.figure(figsize=(10, 5))

plt.plot(
    df["TEMPO_MS"],
    df["UMIDADE"],
    color=ROSA_ESCURO,
    linewidth=4
)

plt.fill_between(
    df["TEMPO_MS"],
    df["UMIDADE"],
    color=ROSA,
    alpha=0.18
)

plt.title("Umidade do Solo ao Longo do Tempo")
plt.xlabel("Tempo (ms)")
plt.ylabel("Umidade (%)")
plt.grid(alpha=0.2)

plt.show()

# TEMPERATURA------------------------------------------
plt.figure(figsize=(8, 5))

plt.hist(
    df["TEMPERATURA"],
    bins=5,
    color=AMARELO_OURO,
    edgecolor="white"
)

plt.title("Distribuição da Temperatura")
plt.xlabel("Temperatura (°C)")
plt.ylabel("Frequência")
plt.grid(alpha=0.2)

plt.show()

# PH-------------------------------------------
plt.figure(figsize=(10, 5))

sns.lineplot(
    data=df,
    x="TEMPO_MS",
    y="PH",
    color=ROSA,
    linewidth=3,
    marker="o"
)

plt.fill_between(
    df["TEMPO_MS"],
    df["PH"],
    color=ROSA_CLARO,
    alpha=0.25
)

plt.title("pH do Solo ao Longo do Tempo")
plt.xlabel("Tempo (ms)")
plt.ylabel("pH")
plt.grid(alpha=0.2)

plt.show()

# NUTRIENTES------------------------------------------
npk = {
    "Nitrogênio": df["NITROGENIO"].mean() * 100,
    "Fósforo": df["FOSFORO"].mean() * 100,
    "Potássio": df["POTASSIO"].mean() * 100
}

plt.figure(figsize=(8, 5))

plt.bar(
    npk.keys(),
    npk.values(),
    color=[ROSA, AMARELO_OURO, ROSA_ESCURO]
)

plt.title("Disponibilidade Média dos Nutrientes")
plt.xlabel("Nutrientes")
plt.ylabel("Presença (%)")
plt.ylim(0, 100)
plt.grid(axis="y", alpha=0.2)

plt.show()

# IRRIGAÇÃO---------------------------------------------
ligada = (df["BOMBA"] == 1).sum()
desligada = (df["BOMBA"] == 0).sum()

plt.figure(figsize=(7, 7))

plt.pie(
    [ligada, desligada],
    labels=["Ligada", "Desligada"],
    colors=[ROSA_ESCURO, AMARELO],
    autopct="%1.1f%%",
    startangle=90,
    wedgeprops={"width": 0.45, "edgecolor": "white"}
)

plt.title("Status da Irrigação")

plt.show()

# SUGESTÃO/FEEDBACK---------------------------------------------
ultima_umidade = df["UMIDADE"].iloc[-1]
ultima_chuva = df["PREVISAO_CHUVA"].iloc[-1]

print("\n ˙⋆✮ SUGESTÃO AUTOMÁTICA")

if ultima_umidade < 40 and ultima_chuva == 0:
    print("Solo com baixa umidade. Irrigação recomendada.")
elif ultima_chuva == 1:
    print("Previsão de chuva detectada. Irrigação não necessária.")
else:
    print("Condições adequadas de irrigação.")
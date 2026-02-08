import pandas as pd
import matplotlib.pyplot as plt
import os

CSV = "results.csv"
OUT = "results/figures"
os.makedirs(OUT, exist_ok=True)

df = pd.read_csv(CSV)

group = df.groupby("step")

processed = group["processed"].mean()
true_mean = group["true_unique"].mean()

# ====== Обычный HLL ======
hll_mean = group["hll_est"].mean()
hll_std = group["hll_est"].std()

# ====== Улучшенный HLL ======
imp_mean = group["hll_improved_est"].mean()
imp_std = group["hll_improved_est"].std()


# ================= ГРАФИК 1 =================
# Ft0 vs Nt (обычный HLL)

plt.figure(figsize=(10, 6))
plt.plot(processed, true_mean, label="True $F_0^t$", linewidth=2)
plt.plot(processed, hll_mean, "--", label="Estimate $N_t$", linewidth=2)

plt.xlabel("Processed elements")
plt.ylabel("Number of unique elements")
plt.title("HyperLogLog: $F_0^t$ vs $N_t$")
plt.legend()
plt.grid(True)

plt.savefig(f"{OUT}/hll_comparison.png")
plt.close()


# ================= ГРАФИК 2 =================
# E(Nt) ± σ (обычный HLL)

plt.figure(figsize=(10, 6))
plt.plot(processed, hll_mean, label="E($N_t$)", linewidth=2)
plt.fill_between(
    processed,
    hll_mean - hll_std,
    hll_mean + hll_std,
    alpha=0.3,
    label="±σ"
)

plt.xlabel("Processed elements")
plt.ylabel("Estimate")
plt.title("HyperLogLog: Mean and Uncertainty")
plt.legend()
plt.grid(True)

plt.savefig(f"{OUT}/hll_mean_std.png")
plt.close()


# ================= ГРАФИК 3 =================
# Ft0 vs Nt (улучшенный HLL)

plt.figure(figsize=(10, 6))
plt.plot(processed, true_mean, label="True $F_0^t$", linewidth=2)
plt.plot(processed, imp_mean, "--", label="Improved $N_t$", linewidth=2)

plt.xlabel("Processed elements")
plt.ylabel("Number of unique elements")
plt.title("Improved HyperLogLog: $F_0^t$ vs $N_t$")
plt.legend()
plt.grid(True)

plt.savefig(f"{OUT}/hll_improved_comparison.png")
plt.close()


# ================= ГРАФИК 4 =================
# E(Nt) ± σ (улучшенный HLL)

plt.figure(figsize=(10, 6))
plt.plot(processed, imp_mean, label="E($N_t$)", linewidth=2)
plt.fill_between(
    processed,
    imp_mean - imp_std,
    imp_mean + imp_std,
    alpha=0.3,
    label="±σ"
)

plt.xlabel("Processed elements")
plt.ylabel("Estimate")
plt.title("Improved HyperLogLog: Mean and Uncertainty")
plt.legend()
plt.grid(True)

plt.savefig(f"{OUT}/hll_improved_mean_std.png")
plt.close()

print("All 4 plots saved to results/figures/")

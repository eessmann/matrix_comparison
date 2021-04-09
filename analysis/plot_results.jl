#=
plot_results:
- Julia version:
- Author: erich
- Date: 2021-04-09
=#
using CSV, DataFrames, Plots

sym_data =
    CSV.File(
        "cmake-build-release-llvm/src/csv_symm_results.csv",
        normalizenames = true,
    ) |> DataFrame

labels = ["Eigen" "Blaze" "GSL"]
plt1 = plot(
    sym_data.size,
    [sym_data.Eigen sym_data.TNT sym_data.GSL],
    plot_title = "Symmetric Matrix Eigenvalues",
    xlabel = "Matrix size, N",
    ylabel = "Time elasped, μs",
    yaxis = :log10,
    label = labels,
    legend = :topleft,
)
savefig(plt1, "symm_results.pdf")

asym_data =
    CSV.File(
        "cmake-build-release-llvm/src/csv_asymm_results.csv",
        normalizenames = true,
    ) |> DataFrame

plt2 = plot(
    sym_data.size,
    [asym_data.Eigen asym_data.Blaze asym_data.GSL],
    plot_title = "Asymmetric Matrix Eigenvalues",
    xlabel = "Matrix size, N",
    ylabel = "Time elasped, μs",
    yaxis = :log10,
    label = labels,
    legend = :topleft,
)
savefig(plt2, "asymm_results.pdf")

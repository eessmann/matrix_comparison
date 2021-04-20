#=
plot_results:
- Julia version:
- Author: erich
- Date: 2021-04-09
=#
using CSV, DataFrames, Plots, SQLite
db = SQLite.DB("analysis/data.sqlite")

sym_data =
    CSV.File(
        "cmake-build-release-aocc/src/csv_symm_results_2021-04-15-15-16-44.csv",
        normalizenames = true,
    ) |> DataFrame

labels = ["Eigen" "Blaze" "GSL" "xtensor"]
plt1 = plot(
    sym_data.size,
    [sym_data.Eigen sym_data.Blaze sym_data.GSL sym_data.Xtensor],
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
        "cmake-build-release-aocc/src/csv_asymm_results_2021-04-15-15-22-21.csv",
        normalizenames = true,
    ) |> DataFrame

plt2 = plot(
    asym_data.size,
    [asym_data.Eigen asym_data.Blaze asym_data.GSL asym_data.Xtensor],
    plot_title = "Asymmetric Matrix Eigenvalues",
    xlabel = "Matrix size, N",
    ylabel = "Time elasped, μs",
    yaxis = :log10,
    label = labels,
    legend = :topleft,
)
savefig(plt2, "asymm_results.pdf")

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
        "cmake-build-release-llvm/src/csv_symm_results_2021-04-09-19-25-38.csv",
        normalizenames = true,
        ignorerepeated = true,
        delim = ",\t",
    ) |> DataFrame

table_name = sym_data |> SQLite.load!(db, "symmetric")

labels = ["Eigen" "Blaze" "GSL"]
plt1 = plot(
    sym_data.size,
    [sym_data.Eigen sym_data.Blaze sym_data.GSL],
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
        "cmake-build-release-llvm/src/csv_asymm_results_2021-04-09-19-30-09.csv",
        normalizenames = true,
    ) |> DataFrame

table_name = asym_data |> SQLite.load!(db, "asymmetric")


plt2 = plot(
    asym_data.size,
    [asym_data.Eigen asym_data.Blaze asym_data.GSL],
    plot_title = "Asymmetric Matrix Eigenvalues",
    xlabel = "Matrix size, N",
    ylabel = "Time elasped, μs",
    yaxis = :log10,
    label = labels,
    legend = :topleft,
)
savefig(plt2, "asymm_results.pdf")

#=
plot_results:
- Julia version:
- Author: erich
- Date: 2021-04-09
=#
using CSV, DataFrames, Plots, SQLite, BenchmarkTools, LinearAlgebra

function generate_plots(fp,pl_title, plot_name)
    sym_data = CSV.File(
        fp,
        normalizenames = true,
    ) |> DataFrame

    labels = ["Eigen" "Blaze" "GSL" "xtensor"]
    plt = plot(
        sym_data.size,
        [sym_data.Eigen sym_data.Blaze sym_data.GSL sym_data.Xtensor],
        plot_title = pl_title,
        xlabel = "Matrix size, N",
        ylabel = "Time elasped, μs",
        yaxis = :log10,
        label = labels,
        legend = :topleft)
    savefig(plt, plot_name)
    
end

function run_Julia_sym(fp)
    sym_data = CSV.File(
        fp,
        normalizenames = true,
    ) |> DataFrame

    function sym_invert(n::Int, alpha::Float64)
        julia_pie = ones(n, n) + alpha * I
        function inner_loop(mat::Matrix)
            for i in 1:32 
                inv(mat)
            end
            return mat
        end
        
        res = @benchmark inner_loop($julia_pie)
        return mean(res).time
    end

    times = Vector{Float64}
    for i in 2:256
        append!(times, sym_invert(i, 0.4))
    end

    sym_data[:Julia] = times


end


function main(alpha::Float64)

    run_Julia_sym("cmake-build-release/src/csv_symm_results_2022-02-10-11-57-03.csv")
    
end
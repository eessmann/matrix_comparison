module MatrixComparison

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

    labels = names(sym_data[!, Not(:size)])
    plt = plot(
        sym_data.size,
        Matrix(sym_data[!, Not(:size)]),
        plot_title = pl_title,
        xlabel = "System size, N",
        xaxis = :log10,
        xticks = [1, 5, 10, 50, 100]
        ylabel = "Time elasped, μs",
        yaxis = :log10,
        yticks = [1, 10, 100, 1000],
        label = permutedims(labels),
        legend = :topleft,
        tex_output_standalone = true)
    savefig(plt, plot_name)
    
end

function run_Julia_sym()
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

    generate_plots("../cmake-build-release/src/csv_symm_inv_results_2023-04-20-16-57-40.csv")
    
end

end # module MatrixComparison

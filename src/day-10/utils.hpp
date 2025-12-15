#pragma once

#include <fstream>
#include <glpk.h>
#include <string>
#include <sstream>
#include <vector>
#include <queue>

void parseManual(std::ifstream& input, 
                 std::vector<std::vector<bool>>& indicators, 
                 std::vector<std::vector<std::vector<int>>>& wiring, 
                 std::vector<std::vector<int>>& jrequirements)
{
    std::string machine;
    while (std::getline(input, machine))
    {
        std::stringstream ss(machine);
        std::vector<bool> indicator;
        std::vector<std::vector<int>> buttons;
        std::vector<int> joltage;

        std::string token;

        // Parse indicator lights
        std::getline(ss, token, '[');
        std::getline(ss, token, ']');
        for (char c : token)
        {
            if (c == '.') indicator.push_back(false);
            else if (c == '#') indicator.push_back(true);
        }

        // Parse button wiring schematics
        while (true)
        {
            char ch;
            ss >> ch;
            if (!ss) break;

            if (ch == '(')
            {
                std::vector<int> button;
                std::string inside;
                std::getline(ss, inside, ')');

                std::stringstream nums(inside);
                std::string num;
                while (std::getline(nums, num, ',')) button.push_back(std::stoi(num));
                buttons.push_back(button);
            }
            else if (ch == '{') break;
        }

        // Parse joltage requirements
        ss >> token;  
        std::getline(ss, token, '}');
        std::stringstream nums(token);
        std::string num;

        while (std::getline(nums, num, ',')) joltage.push_back(std::stoi(num));

        indicators.push_back(indicator);
        wiring.push_back(buttons);
        jrequirements.push_back(joltage);
    }
}

long partI(const std::vector<std::vector<bool>>& indicators, 
           const std::vector<std::vector<std::vector<int>>>& wiring)
{
    long minimum = 0;

    for (int i = 0; i < indicators.size(); ++i)
    {
        uint64_t target = 0;
        int target_size = indicators[i].size();
        for (bool b : indicators[i]) target = (target << 1) | b;

        // Build button masks
        std::vector<uint64_t> button_masks;
        button_masks.reserve((wiring[i].size()));
        for (auto& button : wiring[i]) 
        {
            uint64_t mask = 0;
            for (int j : button) mask |= (1ULL << (target_size - j - 1));
            button_masks.push_back(mask);
        }

        // BFS with dp[state] = minimum presses to reach state
        std::vector<int> dp(1 << target_size, -1);
        std::queue<uint64_t> q;
        q.push(0);
        dp[0] = 0;
        while (!q.empty()) 
        {
            uint64_t state = q.front(); 
            q.pop();
            int presses = dp[state];
            for (uint64_t mask : button_masks) 
            {
                uint64_t new_state = state ^ mask;
                if (dp[new_state] < 0) 
                {
                    dp[new_state] = presses + 1;
                    q.push(new_state);
                }
            }
        }

        minimum += dp[target];
    }

    return minimum;
}

long partII(const std::vector<std::vector<std::vector<int>>>& wiring,
            const std::vector<std::vector<int>>& jrequirements)
{
    long minimum = 0;

    for (int i = 0; i < jrequirements.size(); ++i)
    {
        int cols = wiring[i].size();
        int rows = jrequirements[i].size();

        // transform data from wiring[i] to binary vectors. For example, if we have 6 indicators and one button does (1,4,6) -> [0 1 0 0 1 0 1]
        std::vector<std::vector<int>> buttons(cols, std::vector<int>(rows, 0));
        for (int j = 0; j < cols; ++j)
        {
            for (int el : wiring[i][j]) buttons[j][el] = 1;
        }

        glp_prob *lp = glp_create_prob();
        glp_term_out(GLP_OFF); // there is a lot of terminal output otherwise
        glp_set_obj_dir(lp, GLP_MIN);

        // --- VARIABLES --- Every c_i that multiplies each button (ie a binary vector of presses), as many variables as buttons
        glp_add_cols(lp, cols);
        for (int j = 0; j < cols; ++j) 
        {
            glp_set_col_kind(lp, j+1, GLP_IV); // integer variable
            glp_set_col_bnds(lp, j+1, GLP_LO, 0.0, 0.0); // x_j >= 0
            glp_set_obj_coef(lp, j+1, 1.0); // minimize sum x_j
        }

        // --- CONSTRAINTS --- The joltage requirements are fixed, as many constraints as requirements
        glp_add_rows(lp, rows);
        for (int k = 0; k < rows; ++k) glp_set_row_bnds(lp, k+1, GLP_FX, jrequirements[i][k], jrequirements[i][k]);

        // Matrix storage for GLPK (1-based)
        // A[k][j] = buttons[j][k]
        std::vector<int> ia(1 + cols*rows);
        std::vector<int> ja(1 + cols*rows);
        std::vector<double> ar(1 + cols*rows);

        int idx = 1;
        for (int k = 0; k < rows; ++k) 
        {
            for (int j = 0; j < cols; ++j) 
            {
                ia[idx] = k+1;
                ja[idx] = j+1;
                ar[idx] = buttons[j][k];
                idx++;
            }
        }

        glp_load_matrix(lp, idx-1, ia.data(), ja.data(), ar.data());

        // --- SOLVE --- Mixed integer problem with branch-and-cut method
        glp_iocp parm;
        glp_init_iocp(&parm);
        parm.presolve = GLP_ON;

        // assume a unique solution exists
        glp_intopt(lp, &parm);
        minimum += glp_mip_obj_val(lp);

        glp_delete_prob(lp);
    }

    return minimum;
}

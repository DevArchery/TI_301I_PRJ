//
// Created by steph on 23/11/2025.
//
/*
int gcd(int *vals, int nbvals)
{*/
    /*
    IN: Pointer to an array of integers and number of values in the array
    OUT: The greatest common divisor of all values in the array
    Usage: Used in Markov graph analysis to determine the period of a class by finding the GDC of cycle lengths
     */
/*
    if (nbvals == 0) return 0;          // If no values, return 0
    int result = vals[0];               // Initialize result with first value
    for (int i = 1; i < nbvals; i++) {  // Loop through all other values
        int a = result;
        int b = vals[i];
        while (b != 0) {                // Euclidean algorithm for GCD
            int temp = b;
            b = a % b;                  // Replace b with reminder of a divided by b
            a = temp;                   // Swap values
        }
        result = a;                     // Update with current GCD
    }
    return result;                      // Return GDC of all values
}

int getPeriod(t_matrix sub_matrix)
{*/
    /*
    IN: Submatrix representing the transitions of a strongly connected component (class)
    OUT: Period of the class (int), computed as the greatest common divisor of all cycle lengths
    Usage: Used in Markov graph analysis to determine if a class is periodic by checking when states return to themselves
     */
/*
    int n = sub_matrix.rows;                                // Number of vertices in the submatrix
    int *periods = (int *)malloc(n * sizeof(int));     // Array to store detected cycle lengths
    int period_count = 0;                                   // Counter for number of cycles found
    int cpt = 1;
    t_matrix power_matrix = createEmptyMatrix(n);           // Matrix to hold powers of sub_matrix
    t_matrix result_matrix = createEmptyMatrix(n);          // Temporary matrix for multiplication
    copyMatrix(power_matrix, sub_matrix);                   // Start with sub_matrix (power 1)

    for (cpt = 1; cpt <= n; cpt++)                          // Compute powers up to n
    {
        int diag_nonzero = 0;                               // Check diagonal entries
        for (int i = 0; i < n; i++)                         // Loop through diagonal elements
        {
            if (power_matrix.data[i][i] > 0.0f)             // If diagonal element > 0
            {
                diag_nonzero = 1;                           // Means vertex can return to itself
            }
        }
        if (diag_nonzero) {                                 // If return possible at step cpt
            periods[period_count] = cpt;                    // Record this step as a cycle length
            period_count++;
        }
        multiplyMatrices(power_matrix, sub_matrix, result_matrix); // Compute next power
        copyMatrix(power_matrix, result_matrix);            // Update power_matrix for next iteration
    }
    return gcd(periods, period_count);                      // Period = GCD of all detected cycle lengths
}
*/
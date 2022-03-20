Following was my approach to solve the problem:
1. The training set and test set was loaded. The training set was split into training and validation set to get best value of 'k'.
2. The training and test set was standardized by subtracting mean and dividing by standard deviation.
3. For each test point, its distance to all points in training set were computed and nearest k of them were selected.
4. For each of those k points, squared distance weighted voting was taken so that far away points contribute lesser voting.
5. The class having higher number of votes was chosen as the predicted output for each test point.
6. The same procedure was also applied on the validation set with varying 'k'. The best 'k' was found to be 17.

Important points for execution:
1. The training file must be named as "project2.csv" and test file as "project2_test.csv", else there will be runtime error.
2. Upon successful execution, the output will be stored in file named "19EC30008_P2.out" and they will not be printed on console.
3. The libraries used in the program are pandas (to load csv files) and matplotlib.pyplot (to plot and visualize error on validation set for each k).

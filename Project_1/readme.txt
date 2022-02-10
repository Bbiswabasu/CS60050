To run the program:
	i)   Compile the program with g++ compiler
	ii)  Make sure that the training and test data are kept in the same location as the generated executable program
	iii) Training data file name should be "project1.data" and test data file name should be "project1_test.data"
	iv)  Run the executable program
	v)   Decision tree structure gets stored in file "decision_tree.txt" and predictions stored in "prediction.txt"
	vi)  Replace all the "56" in the test data by "6" as same was done in training data

Decision tree structure:
	i)   Each line of the text contains description of some node
	ii)  It may contain attribute name at that node followed by "=" and the value of the attribute for which current branching will be done
	iii) If the line does not contain "=", it means it is a leaf node and only the decision at that leaf node is printed
	iv)  Number of spaces in the prefix denotes the depth of the node.

Decision tree example:
safety = high
|   persons = 2
|   |   unacc
|   persons = 4
|   |   price = vhigh
|   |   |   maint = vhigh
|   |   |   |   unacc
safety = low
|   unacc

The above tree is interpreted as "safety" is the attribute at root and we branch for value "high"
Then we encounter the attribute "persons" and branch for value "2"
The next line does not contain "=" which means it is a leaf node and the decision is "unacc"
So if (safety=high)&(persons=2) then decision = unacc
Similarly if (safety=high)&(persons=4)&(price=vhigh)&(maint=vhigh) then decision = unacc

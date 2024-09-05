#include <iostream>
using namespace std;

class SparseRow 
{
protected:
	int row;		// Rows
	int col;		// Columns
	int value;

public:
	SparseRow()		// Default Constructor
	{
		row = -1; 
		col = -1; 
		value = 0;
	}

	SparseRow(int r, int c, int v)	// Parametrized Constructor
	{
		row = r;
		col = c;
		value = v;
	}

	void display()	// Print Row
	{
		cout << row << ", " << col << ", " << value << endl;
	}

	friend ostream& operator<<(ostream& s, const SparseRow& r)	// Print Row
	{
		s << r.row << ", " << r.col << ", " << r.value << endl;
		return s;
	}

	// Getter Functions
	int getRow() const 
	{ 
		return row; 
	}

	int getCol() const
	{
		return col;
	}

	int getValue() const
	{
		return value;
	}

	// Setter Functions
	void setRow(int r)
	{ 
		row = r; 
	}

	void setCol(int c)
	{
		col = c;
	}

	void setValue(int v) 
	{ 
		value = v; 
	}
};

class SparseMatrix 
{
protected:
	int noRows;				// Number of Rows
	int noCols;				// Number of Columns
	int commonValue;		// Common Value in the Matrix
	int noNonSparseValues;	// Number of Non-Sparse Values
	SparseRow* myMatrix;	// Input Matrix

public:
	SparseMatrix()	// Default Constructor
	{
		noRows = -1;
		noCols = -1;
		commonValue = -1;
		noNonSparseValues = -1;
		myMatrix = NULL;
	}

	SparseMatrix(int n, int m, int cv, int noNSV)	// Parameterized Constructor
	{
		noRows = n;
		noCols = m;
		commonValue = cv;
		noNonSparseValues = noNSV;
		myMatrix = new SparseRow[noNonSparseValues];
	}

	~SparseMatrix()		// Destructor
	{
		delete[] myMatrix;
		myMatrix = NULL;
	}

	SparseMatrix* Transpose()	// Matrix Transpose
	{
		SparseMatrix* temp = new SparseMatrix(noCols, noRows, commonValue,noNonSparseValues);

		for (int i = 0; i < noNonSparseValues; i++) 	
			temp->myMatrix[i] = SparseRow(myMatrix[i].getCol(), myMatrix[i].getRow(), myMatrix[i].getValue());
		
		return temp;
	}

	SparseMatrix* Multiply(SparseMatrix& M)		// Matrix Multiplication
	{	
		if (noCols != M.noRows) 	
			return NULL;

		// Estimate max possible non-sparse values (worst case: all elements non-zero)
		int maxNonSparseValues = noNonSparseValues * M.noNonSparseValues;

		// Create a new SparseMatrix for the result with enough space for all potential non-zero values
		SparseMatrix* result = new SparseMatrix(noRows, M.noCols, commonValue, maxNonSparseValues);

		// Initialize the number of non-sparse values in the result
		result->setNoNonSparseValues(0);

		// Iterate over each element of the first matrix (this)
		for (int i = 0; i < noNonSparseValues; ++i) {
			SparseRow rowA = myMatrix[i];

			// For each element in the first matrix, find matching rows in the second matrix
			for (int j = 0; j < M.noNonSparseValues; ++j) {
				SparseRow rowB = M.myMatrix[j];

				// Multiplication can only occur when the column of the first matrix equals the row of the second matrix
				if (rowA.getCol() == rowB.getRow()) {
					int product = rowA.getValue() * rowB.getValue();

					// Check if this position already has a value in the result matrix
					bool exists = false;
					for (int k = 0; k < result->getNoNonSparseValues(); ++k) {
						if (result->myMatrix[k].getRow() == rowA.getRow() && result->myMatrix[k].getCol() == rowB.getCol()) {
							result->myMatrix[k].setValue(result->myMatrix[k].getValue() + product);
							exists = true;
							break;
						}
					}

					// If no value exists at this position, add a new non-zero element to the result
					if (!exists && product != commonValue) {
						result->myMatrix[result->getNoNonSparseValues()] = SparseRow(rowA.getRow(), rowB.getCol(), product);
						result->setNoNonSparseValues(result->getNoNonSparseValues() + 1);
					}
				}
			}
		}

		// Resize the result matrix to the actual number of non-sparse values found (optional, for optimization)
		SparseMatrix* finalResult = new SparseMatrix(noRows, M.noCols, commonValue, result->getNoNonSparseValues());
		for (int i = 0; i < result->getNoNonSparseValues(); ++i) {
			finalResult->myMatrix[i] = result->myMatrix[i];
		}
		finalResult->setNoNonSparseValues(result->getNoNonSparseValues());

		// Clean up the temporary result matrix
		delete result;

		return finalResult;
	}

	SparseMatrix* Add(SparseMatrix& M) // Matrix Addition
	{
		if (noRows != M.noRows || noCols != M.noCols)
			return NULL;

		// Allocate enough space for the result to hold all non-sparse values from both matrices
		SparseMatrix* result = new SparseMatrix(noRows, noCols, commonValue, noNonSparseValues + M.noNonSparseValues);

		int i = 0, j = 0, resultIndex = 0;

		// Loop through both matrices and add the values at the same position
		while (i < noNonSparseValues && j < M.noNonSparseValues)
		{
			SparseRow rowA = myMatrix[i];
			SparseRow rowB = M.myMatrix[j];

			if (rowA.getRow() == rowB.getRow() && rowA.getCol() == rowB.getCol())
			{
				int sum = rowA.getValue() + rowB.getValue();

				if (sum != commonValue)
				{
					result->myMatrix[resultIndex++] = SparseRow(rowA.getRow(), rowA.getCol(), sum);
				}

				i++;
				j++;
			}

			else if (rowA.getRow() < rowB.getRow() || (rowA.getRow() == rowB.getRow() && rowA.getCol() < rowB.getCol()))
			{
				result->myMatrix[resultIndex++] = rowA;
				i++;
			}

			else
			{
				result->myMatrix[resultIndex++] = rowB;
				j++;
			}
		}

		// Copy remaining elements from the first matrix (if any)
		while (i < noNonSparseValues)
		{
			result->myMatrix[resultIndex++] = myMatrix[i++];
		}

		// Copy remaining elements from the second matrix (if any)
		while (j < M.noNonSparseValues)
		{
			result->myMatrix[resultIndex++] = M.myMatrix[j++];
		}

		// Update the actual number of non-sparse values in the result matrix
		result->setNoNonSparseValues(resultIndex);
		return result;
	}


	friend ostream& operator<< (ostream& s, const SparseMatrix& sm)		// Display the Matrix
	{
		for (int i = 0; i < sm.noNonSparseValues; i++) 
			s << sm.myMatrix[i];
		
		return s;
	}

	void displayMatrix()	// Display the Matrix in Normal Format
	{
		for (int i = 0; i < noRows; i++)
		{
			for (int j = 0; j < noCols; j++)
			{

				bool found = false;
				for (int k = 0; k < noNonSparseValues; k++)
				{
					if (myMatrix[k].getRow() == -1 || myMatrix[k].getCol() == -1)
						continue;

					if (myMatrix[k].getRow() == i && myMatrix[k].getCol() == j)
					{
						found = true;
						cout << myMatrix[k].getValue()<<" ";
						break;
					}
				}

				if (!found)
				{
					cout << "0 ";
				}
			}

			cout << endl;
		}

		
	}

	void setRow(int row, int i, int j, int val)
	{
		myMatrix[row].setRow(i);
		myMatrix[row].setCol(j);
		myMatrix[row].setValue(val);
	}

	// Getter Functions
	int getRows() const 
	{
		return noRows;
	}

	int getCols() const 
	{
		return noCols;
	}

	int getCommonValue() const 
	{
		return commonValue;
	}

	int getNoNonSparseValues() const 
	{
		return noNonSparseValues;
	}
	
	// Setter Functions
	void setRows(int rows) {
		noRows = rows;
	}

	void setCols(int cols) {
		noCols = cols;
	}

	void setCommonValue(int cv) 
	{
		commonValue = cv;
	}

	void setNoNonSparseValues(int nsv) 
	{
		noNonSparseValues = nsv;
	}
};


int main() 
{
	int n, m, cv, noNSV;
	SparseMatrix* temp = NULL;

	cin >> n >> m >> cv >> noNSV;
	SparseMatrix* firstOne = new SparseMatrix(n, m, cv, noNSV);
	
	int val = cv;
	int count = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			cin >> val;

			if (val != cv)
			{
				firstOne->setRow(count, i, j, val);
				count++;
			}
		}
	}

	cin >> n >> m >> cv >> noNSV;
	SparseMatrix* secondOne = new SparseMatrix(n, m, cv, noNSV);

	val = cv;
	count = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			cin >> val;

			if (val != cv)
			{
				secondOne->setRow(count, i, j, val);
				count++;
			}
		}
	}
	
	cout << "First one in sparse matrix format" << endl;
	cout << (*firstOne);
	cout << "After Transpose" << endl;
	cout << (*(*firstOne).Transpose());
	cout << "First one in matrix format" << endl;
	(*firstOne).displayMatrix();

	cout << "Second one in sparse matrix format" << endl;
	cout << (*secondOne);
	cout << "After Transpose" << endl;
	cout << (*(*secondOne).Transpose());
	cout << "Second one in matrix format" << endl;
	(*secondOne).displayMatrix();

	cout << "Matrix Addition result" << endl;
	temp = firstOne->Add(*secondOne);

	if (temp == NULL)
		cout << "Matrix addition is not possible" << endl;
	
	else
		(*temp).displayMatrix();

	cout << "Matrix multiplication result" << endl;
	temp = firstOne->Multiply(*secondOne);

	if (temp == NULL)
		cout << "Matrix multiplication is not possible" << endl;

	else
		(*temp).displayMatrix();

	return 0;
}

/*
	LLM and GitHub Copilot Usage Documentation:
			Github Copilot as well as Microsoft Copilot was used by me in this project to understand the basic working of multiplication and addition in SMR.
		The prompt I provided it was: Explain how Multiplication and Addition of two Matrices that are in SMR work. It explained the basic working and what techniques
		could be used for it. It also provided a basic code in Python regarding it which served as a hint for me.
		The main reason I chose this prompt was to understand the basic working of the functions so that I would know the proper way of coding it.
			I also used LLMs to fix some bugs that I was having. I was having some invalid values in the resultant matrix and after analyzing using the AI models, I was able
		to identify the issue that I had which revolved around the incorrect size of the SMR.

	Debugging and Testing Plan:
		First of all, I tested if the input method was working or not. I used the file input method mentioned in the project description and at the start I had some issues but they
		easily resolved by careful analysis. After that I coded the print functions and tested them by matching the output. Then I coded the add and multiply function and tested them by
		checking all the inputs and outputs. Moreover, I made some of my own test cases and checked if the answer is as it should be.

	By following all of these steps and paying close attention to debugging and testing on time, I was able to fully complete the project.

*/

#include <iostream>
using namespace std;

//define all your classes here (as given above)
//write the methods after the class definition

class SparseRow {
    protected:
        int row; //Row#
        int col; //Column#
        int value; //We will assume that all our values will be integers
    public:
        SparseRow (); //default constructor; row=-1;col=-1;value=0
        void display(); // print Row#, Column#, value. Fixed display format to "Void" using CoPilot.
        friend ostream& operator<<(ostream& s, const SparseRow& sr); // print Row#, Column#, value. Fixed display format using CoPilot.
        //other methods that are necessary such as get and set
        void setRow(int r); //Set the row number
        void setCol(int c); //Set the column number
        void setValue(int v); //Set the value
        int getRow() const; //Get the row number
        int getCol() const; //Get the column number
        int getValue() const; //Get the value 
};

// Constructor definition
SparseRow::SparseRow() {
    row = -1;
    col = -1;
    value = 0;
}

// Display method definition
void SparseRow::display() {
    cout << "Row#: " << row << ", Column#: " << col << ", Value: " << value << endl;
}

// Overloaded << operator definition
ostream& operator<<(ostream& s, const SparseRow& sr) {
    s << "Row#: " << sr.getRow() << ", Column#: " << sr.getCol() << ", Value: " << sr.getValue(); 
    return s;
}

// Setter methods definitions
void SparseRow::setRow(int r) {
    row = r;
}

void SparseRow::setCol(int c) {
    col = c;
}

void SparseRow::setValue(int v) {
    value = v;
}
// Getter methods definitions
int SparseRow::getRow() const {
    return row;
}

int SparseRow::getCol() const {
    return col;
}

int SparseRow::getValue() const {
    return value;
}

class SparseMatrix {
    protected:
        int noRows; //Number of rows of the original matrix
        int noCols; //Number of columns of the original matrix
        int commonValue; //read from input
        int noNonSparseValues;
        SparseRow* myMatrix; //Array
    public:
        SparseMatrix ();
        SparseMatrix (int n, int m, int cv);
        SparseMatrix* Transpose (); //Matrix Transpose
        SparseMatrix* Multiply (SparseMatrix& M);
        SparseMatrix* Add (SparseMatrix& M);
        friend ostream& operator<< (ostream& s, const SparseMatrix& sm); //print the matrix in sparse format.
        void displayMatrix(); //Display the matrix in its original format. Fixed display format to "Void" using CoPilot.
        //other methods that are necessary such as get and set
        void setNoRows(int n); //Set the number of rows
        void setNoCols(int m); //Set the number of columns
        void setCommonValue(int cv); //Set the common value
        void setNoNonSparseValues(int nsv); //Set the number of non-sparse values
        int getNoRows(); //Get the number of rows
        int getNoCols(); //Get the number of columns
        int getCommonValue(); //Get the common value
        int getNoNonSparseValues(); //Get the number of non-sparse values

        //default constructor
        SparseMatrix::SparseMatrix() { 
            noRows = 0;
            noCols = 0;
            commonValue = 0;
            noNonSparseValues = 0;
            myMatrix = nullptr;
        }

        //constructor
        SparseMatrix::SparseMatrix(int n, int m, int cv, int noNSV) { //constructor fixed to include the parameater "noNSV" to match the main method call. This was found using coPilot.
            noRows = n;
            noCols = m;
            commonValue = cv;
            noNonSparseValues = noNSV;
            myMatrix = nullptr;
        }

        //constructor
        SparseMatrix* SparseMatrix::Transpose() {
            SparseMatrix* transposedMatrix = new SparseMatrix(noCols, noRows, commonValue);
            transposedMatrix->noNonSparseValues = noNonSparseValues;
            transposedMatrix->myMatrix = new SparseRow[noNonSparseValues];

            // Transpose the matrix
            int index = 0;
            for (int i = 0; i < noCols; i++) {
                for (int j = 0; j < noNonSparseValues; j++) {
                    if (myMatrix[j].getCol() == i) {
                        transposedMatrix->myMatrix[index].setRow(myMatrix[j].getCol());
                        transposedMatrix->myMatrix[index].setCol(myMatrix[j].getRow());
                        transposedMatrix->myMatrix[index].setValue(myMatrix[j].getValue());
                        index++;
                    }
                }
            }

            return transposedMatrix;
        }

        // Multiply two matrices
        SparseMatrix* SparseMatrix::Multiply(SparseMatrix& M) {
            if (noCols != M.noRows) {
                cout << "Error: Incompatible matrix dimensions for multiplication." << endl;
                return nullptr;
            }
        
            // Multiply the matrices
            SparseMatrix* multipliedMatrix = new SparseMatrix(noRows, M.noCols, commonValue);
            multipliedMatrix->noNonSparseValues = 0;
            multipliedMatrix->myMatrix = new SparseRow[noRows * M.noCols];
        
            for (int i = 0; i < noRows; i++) {
                for (int j = 0; j < M.noCols; j++) {
                    int sum = 0;
                    for (int k = 0; k < noCols; k++) {
                        int value1 = myMatrix[i].getValue(); 
                        int value2 = M.myMatrix[k * M.noCols + j].getValue();
                        sum += value1 * value2;
                    }
                    if (sum != commonValue) {
                        multipliedMatrix->myMatrix[multipliedMatrix->noNonSparseValues].setRow(i);
                        multipliedMatrix->myMatrix[multipliedMatrix->noNonSparseValues].setCol(j);
                        multipliedMatrix->myMatrix[multipliedMatrix->noNonSparseValues].setValue(sum);
                        multipliedMatrix->noNonSparseValues++;
                    }
                }
            }
        
            return multipliedMatrix;
        }

        // Add two matrices
        SparseMatrix* SparseMatrix::Add(SparseMatrix& M) {
            if (noRows != M.noRows || noCols != M.noCols) {
                cout << "Error: Incompatible matrix dimensions for addition." << endl;
                return nullptr;
            }

            SparseMatrix* addedMatrix = new SparseMatrix(noRows, noCols, commonValue);
            addedMatrix->noNonSparseValues = 0;
            addedMatrix->myMatrix = new SparseRow[noNonSparseValues + M.noNonSparseValues];

            int i = 0, j = 0, index = 0;
            while (i < noNonSparseValues && j < M.noNonSparseValues) {
                int row1 = myMatrix[i].getRow();
                int col1 = myMatrix[i].getCol();
                int row2 = M.myMatrix[j].getRow();
                int col2 = M.myMatrix[j].getCol();

                if (row1 < row2 || (row1 == row2 && col1 < col2)) {
                    addedMatrix->myMatrix[index].setRow(row1);
                    addedMatrix->myMatrix[index].setCol(col1);
                    addedMatrix->myMatrix[index].setValue(myMatrix[i].getValue());
                    i++;
                } else if (row1 > row2 || (row1 == row2 && col1 > col2)) {
                    addedMatrix->myMatrix[index].setRow(row2);
                    addedMatrix->myMatrix[index].setCol(col2);
                    addedMatrix->myMatrix[index].setValue(M.myMatrix[j].getValue());
                    j++;
                } else {
                    int sum = myMatrix[i].getValue() + M.myMatrix[j].getValue();
                    if (sum != commonValue) {
                        addedMatrix->myMatrix[index].setRow(row1);
                        addedMatrix->myMatrix[index].setCol(col1);
                        addedMatrix->myMatrix[index].setValue(sum);
                        addedMatrix->noNonSparseValues++;
                    }
                    i++;
                    j++;
                }
                index++;
            }

            while (i < noNonSparseValues) {
                addedMatrix->myMatrix[index].setRow(myMatrix[i].getRow());
                addedMatrix->myMatrix[index].setCol(myMatrix[i].getCol());
                addedMatrix->myMatrix[index].setValue(myMatrix[i].getValue());
                i++;
                index++;
            }

            while (j < M.noNonSparseValues) {
                addedMatrix->myMatrix[index].setRow(M.myMatrix[j].getRow());
                addedMatrix->myMatrix[index].setCol(M.myMatrix[j].getCol());
                addedMatrix->myMatrix[index].setValue(M.myMatrix[j].getValue());
                j++;
                index++;
            }

            addedMatrix->noNonSparseValues = index;

            return addedMatrix;
        }

        // Overloaded << operator definition. Made it a friend function to access protected members.
        friend ostream& operator<<(ostream& s, const SparseMatrix& sm) {
            for (int i = 0; i < sm.noNonSparseValues; i++) {
                s << sm.myMatrix[i] << endl;
            }
            return s;
        }

        // Display method definition
        void SparseMatrix::displayMatrix() {
            int index = 0;
            for (int i = 0; i < noRows; i++) {
                for (int j = 0; j < noCols; j++) {
                    if (index < noNonSparseValues && myMatrix[index].getRow() == i && myMatrix[index].getCol() == j) {
                        cout << myMatrix[index].getValue() << " ";
                        index++;
                    } else {
                        cout << commonValue << " ";
                    }
                }
                cout << endl;
            }
        }

        // Setter methods definitions
        void SparseMatrix::setNoRows(int n) {
            noRows = n;
        }

        void SparseMatrix::setNoCols(int m) {
            noCols = m;
        }

        void SparseMatrix::setCommonValue(int cv) {
            commonValue = cv;
        }

        void SparseMatrix::setNoNonSparseValues(int nsv) {
            noNonSparseValues = nsv;
        }

        // Getter methods definitions
        int SparseMatrix::getNoRows() {
            return noRows;
        }

        int SparseMatrix::getNoCols() {
            return noCols;
        }

        int SparseMatrix::getCommonValue() {
            return commonValue;
        }

        int SparseMatrix::getNoNonSparseValues() {
            return noNonSparseValues;
        }

};

int main () {

int n, m, cv, noNSV;
SparseMatrix* temp;

cin >> n >> m >> cv >> noNSV;
SparseMatrix* firstOne = new SparseMatrix(n, m, cv);

//Write the Statements to read in the first matrix
cin >> n >> m >> cv >> noNSV;
SparseMatrix* secondOne = new SparseMatrix(n, m, cv);

//Write the Statements to read in the second matrix
cout << "First one in matrix format" << endl;
(*firstOne).displayMatrix();

cout << "First one in sparse matrix format" << endl;
cout << (*firstOne);

cout << "Second one in matrix format" << endl;
(*secondOne).displayMatrix();

cout << "Second one in sparse matrix format" << endl;
cout << (*secondOne);

cout << "Transpose of the first one in matrix" << endl;
cout << (*(*firstOne).Transpose());

cout << "Matrix Addition Result" << endl;

temp = (*firstOne).Add(*secondOne); //Fixed the method call from "Addition" to "Add" class "SparseMatrix" has no member "Addition" to match the method definition. This was found using coPilot.
cout << temp;
(*temp).displayMatrix();

cout << "Matrix Multiplication Result" << endl;

temp = (*firstOne).Multiply(*secondOne); //Dereference the pointer when passing it as an argument. Bug and fix by coPilot.
cout << temp;
(*temp).displayMatrix();
}
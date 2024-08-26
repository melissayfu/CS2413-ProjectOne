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
        display(); // print Row#, Column#, value
        ostream& operator<< (ostream& s, const SparseRow);
        //other methods that are necessary such as get and set
        void setRow(int r); //Set the row number
        void setCol(int c); //Set the column number
        void setValue(int v); //Set the value
        int getRow(); //Get the row number
        int getCol(); //Get the column number
        int getValue(); //Get the value
};

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
        ostream& operator<< (ostream& s, const SparseMatrix& sm);
        displayMatrix (); //Display the matrix in its original format
        //other methods that are necessary such as get and set
};

int main () {

int n, m, cv, noNSV;
SparseMatrix* temp;

cin >> n >> m >> cv >> noNSV;
SparseMatrix* firstOne = new SparseMatrix(n, m, cv, noNSV);

//Write the Statements to read in the first matrix
cin >> n >> m >> cv >> noNSV;
SparseMatrix* secondOne = new SparseMatrix(n, m, cv, noNSV);

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

temp = (*(*firstOne).Addition(secondOne));
cout << temp;
(*temp).displayMatrix();

cout << "Matrix Multiplication Result" << endl;

temp = (*(*firstOne).Multiply(secondOne));
cout << temp;
(*temp).displayMatrix();
}
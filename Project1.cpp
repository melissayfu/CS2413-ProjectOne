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
        void display(); // print Row#, Column#, value. Fixed display format using CoPilot.
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
        ostream& operator<< (ostream& s, const SparseMatrix& sm);
        displayMatrix (); //Display the matrix in its original format
        //other methods that are necessary such as get and set
        void setNoRows(int n); //Set the number of rows
        void setNoCols(int m); //Set the number of columns
        void setCommonValue(int cv); //Set the common value
        void setNoNonSparseValues(int nsv); //Set the number of non-sparse values
        int getNoRows(); //Get the number of rows
        int getNoCols(); //Get the number of columns
        int getCommonValue(); //Get the common value
        int getNoNonSparseValues(); //Get the number of non-sparse values



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
#include <iostream>
#include <vector>

template <class T> class Table{
	
public:
	//	Typedefs
	typedef unsigned int size_type;

	//	Constructor
	Table(){this -> create();}
	Table(size_type rows, size_type cols, const T& t);
	//	Assignment operator
	Table(const Table& t){ copy(t); }
	Table& operator=(const Table& t);
	//	Destructor
	~Table();

	//	Accessor and modifier
	T get(size_type row, size_type col) const;
	void set(size_type row, size_type col, T new_value);
	size_type numRows() const { return rows; }
	size_type numColumns() const { return cols; }
	void print() const;
	void destroy();

	//	Member functions and other operators
	void push_back_row(std::vector<T>& new_row);
	void push_back_column(std::vector<T>& new_column);
	void pop_back_row();
	void pop_back_column();

private:
	//	Private member functions
	void create();
	void create(size_type rows, size_type cols, const T& value);
	void copy(const Table<T>& t);

	//	Private variables
	T **values;//	Pointer to the first location in the allocated array
	size_type rows;//	Number of rows
	size_type cols;//	Number of columns
};


//
template <class T> 
Table<T>::Table(size_type rows, size_type cols, const T& t){
	this -> create(rows, cols, t);
}

//	Create an empty table (null pointer for value)
template <class T> void Table<T>::create(){
	values = NULL;
	rows = 0;
	cols = 0;
}

//	Create a table filled with value provided
template <class T> 
void Table<T>::create(size_type row, size_type col, const T& value){
	//	Create a new 2D dynamic array and fill it with value
	values = new T*[row];
	for (int i=0; i<row; i++){
		values[i] = new T[col];
		for (int j=0; j<col; j++){
			values[i][j] = value;
		}
	}
	//	Assign the other two parameter to private variables
	rows = row;
	cols = col;
}

//	Helper function that delete all the heap object assigned
template <class T> void Table<T>::destroy(){
	for (int i=0; i<rows; i++){
		delete []values[i];
	}
	delete []values;
}

//Destructor
template <class T> Table<T>::~Table(){
	destroy();
}

//	Entering the row and column and return the value in that position
template <class T> T Table<T>::get(size_type row, size_type col) const {
	//If any of the entries is wrong, report error
	if (row < 0 || row >= rows){
		std::cerr << "Invalid row!" << std::endl;
		exit(1);
	}
	if (col < 0 || col >= cols){
		std::cerr << "Invalid column!" << std::endl;
		exit(1);
	}
	return values[row][col]; 
}

//	Entering the row, column and the new value assigned 
//	fill the position with the value
template<class T>void Table<T>::set(size_type row,size_type col,T new_value){
	if (row < 0 || row >= rows){
		std::cerr << "Invalid row!" << std::endl;
		exit(1);
	}
	if (col < 0 || col >= cols){
		std::cerr << "Invalid column!" << std::endl;
		exit(1);
	}
	values[row][col]=new_value;
}

//	Print the table
template <class T> void Table<T>::print() const {
	for (int i=0; i<rows; i++){
		for (int j=0; j<cols; j++){
			std::cout << values[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}

//	Assign one table to another, avoiding duplicate copying
template <class T> Table<T>& Table<T>::operator=(const Table<T>& t){
	if (this != &t){
		destroy();
		this -> copy(t);
	}
	return *this;
}

//	Create the table as a copy of the given table
template <class T> void Table<T>::copy(const Table<T>& t){
	this -> rows = t.numRows();
	this -> cols = t.numColumns();
	this -> values = new T*[rows];
	//	Copy all the content from the original table
	for (int i=0; i<rows; i++){
		this -> values[i] = new T[cols];
		for (int j=0; j<cols; j++){
			values[i][j] = t.get(i, j);
		}
	}
}

//	Add a new row to the table with the content inside given vector
template <class T> void Table<T>::push_back_row(std::vector<T>& new_row){
	//	report error if the size of input vector is wrong
	if (cols != new_row.size()){
		std::cerr << "Invalid command!(wrong length of vector)" << std::endl;
		exit(1);
	}
	//	Create a new 2D dynamic array
	T** new_values = new T*[rows + 1];
	//	Copy all data from the original table
	for (int i=0; i<rows; i++){
		new_values[i] = new T[cols];
		for (int j=0; j<cols; j++){
			new_values[i][j] = values[i][j];
		}
	}
	//	Copy all data from the vector to the end of new table
	new_values[rows] = new T[cols];
	for (int i=0; i<cols; i++){
		new_values[rows][i] = new_row[i];
	}
	//Delete
	destroy();
	rows++;
	//Assign the values to the new table
	values = new_values;
}

//	Add a new column to the table with the content inside given vector
template<class T>void Table<T>::push_back_column(std::vector<T>& new_column){
	//	report error if the size of input vector is wrong
	if (rows != new_column.size()){
		std::cerr << "Invalid command!(wrong length of vector)" << std::endl;
		exit(1);
	}
	//	Create a new 2D dynamic array
	T** new_values = new T*[rows];
	//	Copy all data from the original table
	for (int i=0; i<rows; i++){
		new_values[i] = new T[cols + 1];
		for (int j=0; j<cols; j++){
			new_values[i][j] = values[i][j];
		}
	}
	//	Copy all data from the vector to the end of each row
	for (int i=0; i<rows; i++){
		new_values[i][cols] = new_column[i];
	}
	//	Delete
	destroy();
	cols++;
	//	Assign the values to the new table
	values = new_values;
}

//	Remove the last row of the table
template <class T> void Table<T>::pop_back_row(){
	// Report error if it is an empty table
	if (rows == 0){
		std::cerr << "There is no row to pop!" << std::endl;
		exit(1);
	}
	T** new_values = new T*[rows - 1];
	//	Copy all data except last row
	for (int i=0; i<rows - 1; i++){
		new_values[i] = new T[cols];
		for (int j=0; j<cols; j++){
			new_values[i][j] = values[i][j];
		}
	}
	//	Delete
	destroy();
	rows--;
	//	Assign the values to the new table
	values = new_values;
}

//	Remove last element from each row
template <class T> void Table<T>::pop_back_column(){
	//	Report error if the row is empty
	if (cols == 0){
		std::cerr << "There is no column to pop!" << std::endl;
		exit(1);
	}
	T** new_values = new T*[rows];
	//	Copy all data except last column
	for (int i=0; i<rows; i++){
		new_values[i] = new T[cols];
		for (int j=0; j<cols - 1; j++){
			new_values[i][j] = values[i][j];
		}
	}
	//Delete
	destroy();
	cols--;
	//Assign the values to the new table
	values = new_values;
}


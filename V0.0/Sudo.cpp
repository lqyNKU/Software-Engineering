#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <string>
using namespace std;

const int GRID_SIZE = 9;
const int BOX_SIZE = 3;

// �ж�������ĳ��λ���Ƿ�ȫ
bool isSafe(int sudoku[GRID_SIZE][GRID_SIZE], int row, int col, int num) {
	// ������Ƿ�ȫ
	for (int i = 0; i < GRID_SIZE; i++) {
		if (sudoku[row][i] == num) {
			return false;
		}
	}

	// ������Ƿ�ȫ
	for (int i = 0; i < GRID_SIZE; i++) {
		if (sudoku[i][col] == num) {
			return false;
		}
	}

	// ���3x3�����Ƿ�ȫ
	int boxRow = row - row % BOX_SIZE;
	int boxCol = col - col % BOX_SIZE;
	for (int i = 0; i < BOX_SIZE; i++) {
		for (int j = 0; j < BOX_SIZE; j++) {
			if (sudoku[boxRow + i][boxCol + j] == num) {
				return false;
			}
		}
	}

	return true;
}

// �������������
bool solveSudoku(int sudoku[GRID_SIZE][GRID_SIZE]) {
	for (int row = 0; row < GRID_SIZE; row++) {
		for (int col = 0; col < GRID_SIZE; col++) {
			if (sudoku[row][col] == 0) {
				for (int num = 1; num <= GRID_SIZE; num++) {
					if (isSafe(sudoku, row, col, num)) {
						sudoku[row][col] = num;
						if (solveSudoku(sudoku)) {
							return true;
						}
						sudoku[row][col] = 0;
					}
				}
				return false;
			}
		}
	}
	return true;
}

// �����������ļ�
void saveSudokuToFile(int sudoku[GRID_SIZE][GRID_SIZE], const string& filename) {
	ofstream file(filename);
	if (file.is_open()) {
		for (int i = 0; i < GRID_SIZE; i++) {
			for (int j = 0; j < GRID_SIZE; j++) {
				file << sudoku[i][j] << " ";
			}
			file << endl;
		}
		file.close();
	}
	else {
		cout << "�޷����ļ���" << filename << endl;
	}
}

// ������������Ⲣ����
void solveSudokuCounter(int sudoku[GRID_SIZE][GRID_SIZE], int& count) {
	for (int row = 0; row < GRID_SIZE; row++) {
		for (int col = 0; col < GRID_SIZE; col++) {
			if (sudoku[row][col] == 0) {
				for (int num = 1; num <= GRID_SIZE; num++) {
					if (isSafe(sudoku, row, col, num)) {
						sudoku[row][col] = num;
						solveSudokuCounter(sudoku, count);
						sudoku[row][col] = 0;
					}
				}
				return;
			}
		}
	}
	count++; // �ҵ�һ����ʱ������һ
}

// ����������Ƿ�Ψһ
bool isUniqueSolution(int sudoku[GRID_SIZE][GRID_SIZE]) {
	int count = 0; // ��¼���Ľ������
	solveSudokuCounter(sudoku, count);
	return count == 1; // ����������Ϊ1����˵������Ψһ��
}

// �ж������Ƿ����Ψһ��
bool hasUniqueSolution(int sudoku[GRID_SIZE][GRID_SIZE]) {
	// ���������ĸ����������ԭʼ���������޸�
	int tempSudoku[GRID_SIZE][GRID_SIZE];
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			tempSudoku[i][j] = sudoku[i][j];
		}
	}

	// ���ԶԸ����������
	return solveSudoku(tempSudoku) && isUniqueSolution(tempSudoku);
}

// ����������Ϸ
void generateSudoku(int difficulty, int holes, const string& filename) {
	random_device rd;
	mt19937 gen(rd());

	int sudoku[GRID_SIZE][GRID_SIZE] = { 0 };
	int count = 0;

	while (count != 1) {
		solveSudoku(sudoku);
		uniform_int_distribution<> dist(0, GRID_SIZE - 1);

		for (int i = 0; i < holes; i++) {
			int row = dist(gen);
			int col = dist(gen);

			while (sudoku[row][col] == 0) {
				row = dist(gen);
				col = dist(gen);
			}

			sudoku[row][col] = 0;
		}

		count = 0;
		solveSudokuCounter(sudoku, count);
	}

	saveSudokuToFile(sudoku, filename);

	cout << "������Ϸ�����ɲ��������ļ���" << filename << endl;
}

// ���ļ��ж�ȡ�������Ⲣ���
void solveSudokuFromFile(const string& filename, const string& outputFilename) {
	ifstream file(filename);
	if (file.is_open()) {
		int sudoku[GRID_SIZE][GRID_SIZE];

		for (int i = 0; i < GRID_SIZE; i++) {
			for (int j = 0; j < GRID_SIZE; j++) {
				file >> sudoku[i][j];
			}
		}

		file.close();

		solveSudoku(sudoku);

		ofstream outputFile(outputFilename);
		if (outputFile.is_open()) {
			for (int i = 0; i < GRID_SIZE; i++) {
				for (int j = 0; j < GRID_SIZE; j++) {
					outputFile << sudoku[i][j] << " ";
				}
				outputFile << endl;
			}
			outputFile.close();
			cout << "������������⣬�����������ļ���" << outputFilename << endl;
		}
		else {
			cout << "�޷����ļ���" << outputFilename << endl;
		}
	}
	else {
		cout << "�޷����ļ���" << filename << endl;
	}
}

int main() {
	int difficulty, holes;
	string filename, inputFilename, outputFilename;

	cout << "��ѡ��������Ϸ���Ѷȣ�1-5����";
	cin >> difficulty;

	cout << "��������Ϸ���ڿյ�������";
	cin >> holes;

	cout << "������Ҫ�����������Ϸ�ļ�����";
	cin.ignore();
	getline(cin, filename);

	generateSudoku(difficulty, holes, filename);

	cout << "������Ҫ�������������ļ�����";
	getline(cin, inputFilename);

	cout << "������Ҫ������������ļ�����";
	getline(cin, outputFilename);

	solveSudokuFromFile(inputFilename, outputFilename);

	return 0;
}

#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <string>
using namespace std;

const int GRID_SIZE = 9;
const int BOX_SIZE = 3;

// 判断数独中某个位置是否安全
bool isSafe(int sudoku[GRID_SIZE][GRID_SIZE], int row, int col, int num) {
	// 检查行是否安全
	for (int i = 0; i < GRID_SIZE; i++) {
		if (sudoku[row][i] == num) {
			return false;
		}
	}

	// 检查列是否安全
	for (int i = 0; i < GRID_SIZE; i++) {
		if (sudoku[i][col] == num) {
			return false;
		}
	}

	// 检查3x3宫格是否安全
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

// 对数独进行求解
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

// 保存数独到文件
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
		cout << "无法打开文件：" << filename << endl;
	}
}

// 对数独进行求解并计数
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
	count++; // 找到一个解时计数加一
}

// 检查数独解是否唯一
bool isUniqueSolution(int sudoku[GRID_SIZE][GRID_SIZE]) {
	int count = 0; // 记录求解的解的数量
	solveSudokuCounter(sudoku, count);
	return count == 1; // 如果解的数量为1，则说明解是唯一的
}

// 判断数独是否具有唯一解
bool hasUniqueSolution(int sudoku[GRID_SIZE][GRID_SIZE]) {
	// 创建数独的副本，避免对原始数独进行修改
	int tempSudoku[GRID_SIZE][GRID_SIZE];
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			tempSudoku[i][j] = sudoku[i][j];
		}
	}

	// 尝试对副本数独求解
	return solveSudoku(tempSudoku) && isUniqueSolution(tempSudoku);
}

// 生成数独游戏
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

	cout << "数独游戏已生成并保存至文件：" << filename << endl;
}

// 从文件中读取数独问题并求解
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
			cout << "数独问题已求解，结果已输出至文件：" << outputFilename << endl;
		}
		else {
			cout << "无法打开文件：" << outputFilename << endl;
		}
	}
	else {
		cout << "无法打开文件：" << filename << endl;
	}
}

int main() {
	int difficulty, holes;
	string filename, inputFilename, outputFilename;

	cout << "请选择生成游戏的难度（1-5）：";
	cin >> difficulty;

	cout << "请输入游戏中挖空的数量：";
	cin >> holes;

	cout << "请输入要保存的数独游戏文件名：";
	cin.ignore();
	getline(cin, filename);

	generateSudoku(difficulty, holes, filename);

	cout << "请输入要求解的数独问题文件名：";
	getline(cin, inputFilename);

	cout << "请输入要保存求解结果的文件名：";
	getline(cin, outputFilename);

	solveSudokuFromFile(inputFilename, outputFilename);

	return 0;
}

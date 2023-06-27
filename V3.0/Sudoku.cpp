#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <sstream>
#include <typeinfo>

using namespace std;

//9行9列
#define ROW_COUNT 9
#define COL_COUNT 9
int game_board[ROW_COUNT][COL_COUNT] = { 0 };
int row_valid[9];
int col_valid[9];
int grid_valid[9];

void printGameBoard(int n)
{
	//cout << "Succesfully generated the " << n + 1 << " game!" << endl;
	cout << "***************************" << endl;
	for (int i = 0; i < ROW_COUNT; i++)
	{
		for (int j = 0; j < COL_COUNT; j++)
		{
			cout << game_board[i][j] << ' ';
		}
		cout << endl;
	}
	cout << "***************************" << endl;
	cout << endl;
}

void writeFile(string& filename)
{
	ofstream file(filename, std::ios::app);
	if (file.is_open()) {
		for (int i = 0; i < ROW_COUNT; i++) {
			for (int j = 0; j < COL_COUNT; j++) {
				file << game_board[i][j] << " ";
			}
			file << endl;
		}
		file << endl << endl;
		file.close();
	}
	else
		cout << "Can not open the file:" << filename << endl;
}

void readFile(const std::string& filename, std::vector<std::vector<int>>& puzzles)
{
	std::ifstream file(filename);  // 打开文件

	if (!file)
	{
		std::cout << "Error: Failed to open the file." << std::endl;
		return;
	}

	std::string line;
	std::vector<int> puzzle;  // 用于存储当前数独问题的行
	while (std::getline(file, line))  // 逐行读取文件内容
	{
		if (!line.empty())  // 如果当前行不为空
		{
			std::istringstream iss(line);
			int num;
			while (iss >> num)  // 从当前行中逐个读取数字
			{
				puzzle.push_back(num);
			}
		}
		else if (!puzzle.empty())  // 如果当前行为空且当前数独问题不为空
		{
			puzzles.push_back(puzzle);  // 将当前数独问题添加到问题列表中
			puzzle.clear();  // 清空当前数独问题的行
		}
	}

	if (!puzzle.empty())  // 处理最后一个数独问题
	{
		puzzles.push_back(puzzle);
	}

	file.close();  // 关闭文件
}


bool isRowValid(int row, int num)
{
	for (int col = 0; col < COL_COUNT; ++col)
	{
		if (game_board[row][col] == num)
		{
			return false;
		}
	}
	return true;
}

bool isColValid(int col, int num)
{
	for (int row = 0; row < ROW_COUNT; ++row)
	{
		if (game_board[row][col] == num)
		{
			return false;
		}
	}
	return true;
}

bool isGridValid(int row, int col, int num)
{
	int startRow = (row / 3) * 3;
	int startCol = (col / 3) * 3;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (game_board[startRow + i][startCol + j] == num)
			{
				return false;
			}
		}
	}
	return true;
}

bool isValidMove(int row, int col, int num)
{
	return isRowValid(row, num) && isColValid(col, num) && isGridValid(row, col, num);
}


bool solveSudoku()
{
	for (int row = 0; row < ROW_COUNT; ++row)
	{
		for (int col = 0; col < COL_COUNT; ++col)
		{
			if (game_board[row][col] == 0)
			{
				for (int num = 1; num <= 9; ++num)
				{
					if (isValidMove(row, col, num))
					{
						game_board[row][col] = num;
						if (solveSudoku())
						{
							return true;
						}
						game_board[row][col] = 0;
					}
				}
				return false;
			}
		}
	}
	return true;
}


void solve(const std::string& filename)
{
	std::vector<std::vector<int>> puzzles;

	readFile(filename, puzzles);

	for (int i = 0; i < puzzles.size(); i++)
	{
		// 将读取的数独问题赋值给游戏棋盘
		for (int x = 0; x < ROW_COUNT; ++x)
		{
			for (int y = 0; y < COL_COUNT; ++y)
			{
				game_board[x][y] = puzzles[i][x * ROW_COUNT + y];
			}
		}

		// 尝试解决数独问题
		bool success = solveSudoku();

		if (success)
		{
			std::cout << "Successfully solved the sudoku puzzle!" <<i+1<< std::endl;
			// 将结果写入文件
			std::string resultFilename = "sudoku.txt";
			writeFile(resultFilename);
		}
		else
		{
			std::cout << "Failed to solve the sudoku puzzle!" << std::endl;
		}
	}
}






void initValid()
{
	for (int i = 0; i < 9; i++)
	{
		row_valid[i] = 511;  // 将第 i 行的有效性初始化为二进制表示的 511（即全部位为1）
		col_valid[i] = 511;  // 将第 i 列的有效性初始化为二进制表示的 511
		grid_valid[i] = 511;  // 将第 i 个小九宫格的有效性初始化为二进制表示的 511
	}
}



void updateValid(int x, int y, int value)
{
	/*
	该函数用于切换数独游戏中某个位置的数字的有效性，即将其从有效状态切换为无效状态，或者从无效状态切换为有效状态。

	row_valid[x]、col_valid[y] 和 grid_valid[my_grid] 分别表示数独游戏中每一行、每一列和每个小九宫格中数字的有效性的整数表示。

	使用按位异或运算符^ 对对应位置的数字的有效性进行切换。通过左移(value - 1) 位来找到对应数字的位。

	异或运算的性质是当两个操作数的对应位不同时，结果为 1；当两个操作数的对应位相同时，结果为 0。

	切换后的有效性表示将对应数字的位进行反转，即如果原来是有效状态，则变为无效状态；如果原来是无效状态，则变为有效状态。
	*/
	//cout << "511 ^ (1 << value) " << (511 ^ (1 << value) << endl;
	row_valid[x] ^= (1 << (value - 1));  // 切换第 x 行的有效性中对应数字的位
	col_valid[y] ^= (1 << (value - 1));  // 切换第 y 列的有效性中对应数字的位
	int row_grid = x / 3;  // 计算当前位置所在的小九宫格的行索引
	int col_grid = y / 3;  // 计算当前位置所在的小九宫格的列索引
	int my_grid = row_grid * 3 + col_grid;  // 计算当前位置所在的小九宫格的索引
	grid_valid[my_grid] ^= (1 << (value - 1));  // 切换当前小九宫格的有效性中对应数字的位
}


void restoreValid(int x, int y, int value)
{
	row_valid[x] |= (1 << (value - 1));  // 将第 x 行的有效性中对应数字的位设置为 1
	col_valid[y] |= (1 << (value - 1));  // 将第 y 列的有效性中对应数字的位设置为 1
	int row_grid = x / 3;  // 计算当前位置所在的小九宫格的行索引
	int col_grid = y / 3;  // 计算当前位置所在的小九宫格的列索引
	int my_grid = row_grid * 3 + col_grid;  // 计算当前位置所在的小九宫格的索引
	grid_valid[my_grid] |= (1 << (value - 1));  // 将当前小九宫格的有效性中对应数字的位设置为 1
}


random_device rd;
mt19937 rng(rd());

bool fillBoard(int x, int y)
{
	/*
	该函数通过递归回溯的方式尝试填充数独棋盘的每个位置，使用位运算和随机化来获取有效数字并进行填充。
	它通过检查每个位置上的行、列和小九宫格的有效数字，然后选择一个有效数字填充，递归调用自身填充下一个位置。
	如果所有位置都成功填充，函数返回true；如果无法成功填充，函数返回false。
	在尝试填充失败时，会恢复当前位置的数字的有效性，以便回溯到前一个位置进行其他尝试。
	*/
	if (x == 9)
	{
		return true;  // 所有行都已经填充完毕，返回true表示成功填充数独棋盘
	}
	if (y == 9)
	{
		return fillBoard(x + 1, 0);  // 当前行已经填充完毕，递归进入下一行的填充
	}

	/* acquire valid values */
	int row_grid = x / 3;  // 计算当前位置所在的小九宫格的行索引
	int col_grid = y / 3;  // 计算当前位置所在的小九宫格的列索引
	int k = row_grid * 3 + col_grid;  // 计算当前位置所在的小九宫格的索引
	int valid = row_valid[x] & col_valid[y] & grid_valid[k];  // 获取当前位置上的有效数字

	int tmp = 1;
	vector<int> valid_value_list;  // 存储当前位置上的有效数字列表
	while (tmp < 10)
	{
		if ((valid & (1 << (tmp - 1))) != 0)  // 判断当前数字tmp是否在有效数字列表中
		{
			valid_value_list.push_back(tmp);  // 将有效数字tmp加入列表
		}
		tmp++;
	}

	shuffle(valid_value_list.begin(), valid_value_list.end(), rng);  // 随机打乱有效数字列表的顺序

	for (int i = 0; i < valid_value_list.size(); i++)
	{
		game_board[x][y] = valid_value_list[i];  // 将当前位置的数字设为有效数字列表中的某个数字
		updateValid(x, y, valid_value_list[i]);  // 更新当前位置所在行、列和小九宫格中数字的有效性

		if (fillBoard(x, y + 1))  // 递归调用填充函数，填充下一个位置
		{
			return true;  // 如果下一个位置及之后的位置都成功填充，则返回true
		}
		else
		{
			restoreValid(x, y, valid_value_list[i]);  // 如果下一个位置填充失败，则恢复当前位置的数字的有效性
		}
	}

	return false;  // 如果所有的有效数字都尝试过仍然无法成功填充数独棋盘，则返回false
}


bool generateSudo()
{
	initValid();
	return fillBoard(0, 0);
}

int generateSudoKu(int count)
{
	if (count < 1 || count > 10000)
	{
		cout << "Error: Invalid parameter for game_count!" << endl;
		return -1;  // 参数 count 的值无效，返回错误码 -1 表示生成失败
	}
	for (int i = 0; i < count; i++)
	{
		cout << endl << "Begin generating the " << i + 1 << " game..." << endl;  // 开始生成第 i+1 个数独游戏
		while (!generateSudo());  // 循环调用 generateSudo 函数，直到成功生成一个数独棋盘
		printGameBoard(i);  // 打印生成的数独棋盘
	}
	return 0;  // 成功生成所有数独棋盘，返回成功码 0
}


void digHoles(int hole_cnt)
{
	/*
	该函数用于在生成的数独棋盘中挖去一定数量的洞。

	函数参数 hole_cnt 指定要挖去的洞的数量。

	使用 uniform_int_distribution 创建一个均匀分布的整数随机数生成器对象 dist，范围为 0 到 80，用于生成随机数。

	使用循环，迭代 hole_cnt 次，即挖去指定数量的洞。

	在每次循环中，生成一个随机数 ran，范围在 0 到 80 之间。

	进入一个循环，检查随机位置上的数字是否已经是空白格（即为 0）。

	如果随机位置上的数字已经是空白格，则继续生成一个新的随机数，直到找到一个非空白格的位置。

	将找到的非空白格的位置上的数字设置为 0，即将其挖去，形成一个洞。

	*/
	uniform_int_distribution<int> dist(0, 80);  // 创建一个均匀分布的整数随机数生成器
	for (int i = 0; i < hole_cnt; i++)
	{
		int ran = dist(rng);  // 生成一个随机数，范围在 0 到 80 之间
		//cout << ran << endl;

		while (game_board[ran / 9][ran % 9] == 0)  // 如果随机位置已经是空白格
		{
			ran = dist(rng);  // 继续生成一个新的随机数
		}
		game_board[ran / 9][ran % 9] = 0;  // 将随机位置上的数字设置为 0，即挖去一个洞
	}
}


int generateSudoGame(int game_count, int mode = 0, int subarg = 0)
{
	for (int i = 0; i < game_count; i++)
	{
		cout << endl << "Begin generating the " << i + 1 << " game..." << endl;  // 开始生成第 i+1 个数独游戏
		while (!generateSudo());  // 循环调用 generateSudo 函数，直到成功生成一个数独棋盘

		switch (mode)
		{
		case 0:  // 指定 -n 参数的情况
		{
			uniform_int_distribution<int> dist(20, 55);
			int ran_hole_cnt = dist(rng);  // 生成一个 20 到 55 之间的随机洞的数量
			cout << ran_hole_cnt << endl;
			digHoles(ran_hole_cnt);  // 挖去随机洞的数量
			break;
		}
		case 1:  // 指定 -n -m "difficulty"mode 的情况
		{
			if (subarg == 1)
			{
				digHoles(20);  // 挖去 20 个洞，难度为简单
			}
			else if (subarg == 2)
			{
				digHoles(35);  // 挖去 35 个洞，难度为中等
			}
			else if (subarg == 3)
			{
				digHoles(55);  // 挖去 55 个洞，难度为困难
			}
			else
			{
				cout << "Error: Invalid parameter for difficulty!" << endl;
				return -1;  // 难度参数 subarg 无效，返回错误码 -1
			}
			break;
		}
		case 2:  // 指定 -n -r "holes" 的情况
		{
			if (subarg < 20 || subarg > 55)
			{
				cout << "Error: Invalid parameter for hole_count!" << endl;
				return -1;  // 洞的数量参数 subarg 无效，返回错误码 -1
			}
			digHoles(subarg);  // 挖去指定数量的洞
			break;
		}
		case 3:  // 指定 -n -u 的情况
		{
			bool uniqueSolution = true;
			for (int x = 0; x < ROW_COUNT; ++x)
			{
				for (int y = 0; y < COL_COUNT; ++y)
				{
					if (game_board[x][y] != 0)
					{
						int value = game_board[x][y];
						game_board[x][y] = 0;  // 将当前位置的数字设置为 0
						initValid();  // 初始化有效性

						if (!solveSudoku())
						{
							uniqueSolution = false;
							break;
						}

						game_board[x][y] = value;  // 恢复当前位置的数字
						initValid();  // 初始化有效性
					}
				}
				if (!uniqueSolution)
				{
					break;
				}
			}

			if (!uniqueSolution)
			{
				i--;  // 如果数独问题没有唯一解，重新生成数独棋盘
				continue;
			}
			uniform_int_distribution<int> dist(20, 55);
			int ran_hole_cnt = dist(rng);  // 生成一个 20 到 55 之间的随机洞的数量
			cout << ran_hole_cnt << endl;
			digHoles(ran_hole_cnt);
			break;
		}
		default:
			break;
		}

		printGameBoard(i);  // 打印生成的数独棋盘
	}
	return 0;  // 成功生成所有数独棋盘，返回成功码 0
}


int main(int argc, char* argv[])
{
	//generateSudoGame(10);
	//printGameBoard(0);
	//generateSudoGame(5, 0);

	if (argc < 2)
	{
		cout << "Error: Invalid Input!" << endl;
		return -1;  // 参数数量不足，返回错误码 -1
	}
	//srand((unsigned)time(NULL));
	string arg1 = argv[1];
	if (arg1 == "-c")
	{
		if (argc == 3)
		{
			string subarg1 = argv[2];
			int count = stoi(subarg1);
			return generateSudoKu(count);  // 生成指定数量的数独游戏
		}
		else
		{
			cout << "Error: Invalid parameter for '-c'!" << endl;
			return -1;  // 参数数量不正确，返回错误码 -1
		}
	}
	else if (arg1 == "-s")
	{
		if (argc == 3) // 解文件中的数独
		{
			std::string filename = argv[2];
			solve(filename);
		}
		else
		{
			cout << "Error: Invalid parameter for '-s'!" << endl;
			return -1;  // 参数数量不正确，返回错误码 -1
		}
	}
	else if (arg1 == "-n")
	{
		string subarg1 = argv[2];
		int game_count = stoi(subarg1);
		// cout << game_count << endl;
		switch (argc)
		{
		case 3:  // 只要求生成游戏数量
		{
			generateSudoGame(game_count);  // 生成指定数量的数独游戏
			break;
		}
		case 4:  // 唯一解 -u
		{
			string arg2 = argv[3];
			bool flag = true;
			if (arg2 == "-u")
			{
				return generateSudoGame(game_count, 3);  // 生成指定数量的数独游戏，并要求具有唯一解
			}
			else
			{
				cout << "Error: Invalid input!" << endl;
				return -1;  // 参数无效，返回错误码 -1
			}
			break;
		}

		case 5:  // -m/-r
		{
			string arg2 = argv[3];
			string subarg2 = argv[4];
			if (arg2 == "-m")
			{
				int difficulty = stoi(subarg2);
				return generateSudoGame(game_count, 1, difficulty);  // 生成指定数量的数独游戏，并指定难度
			}
			else if (arg2 == "-r")
			{
				int hole_count = stoi(subarg2);
				return generateSudoGame(game_count, 2, hole_count);  // 生成指定数量的数独游戏，并指定洞的数量
			}
			else
			{
				cout << "Error: Invalid input!" << endl;
				return -1;  // 参数无效，返回错误码 -1
			}
			break;
		}
		default:
			cout << "Error: Invalid parameter for '-n'!" << endl;
			return -1;  // 参数数量不正确，返回错误码 -1
		}
	}

	return 0;  // 成功执行程序，返回成功码 0
}


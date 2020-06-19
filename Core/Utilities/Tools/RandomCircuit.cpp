#include "Core/Utilities/Tools/RandomCircuit.h"
#include "Core/QuantumCircuit/QGate.h"
#include <sstream>
#include<time.h>

using namespace std;
USING_QPANDA

RandomCircuit::RandomCircuit(QuantumMachine *qvm, QVec &qv)
	:m_qvm(qvm), m_qv(qv)
{
	m_set_layer_func_vec.push_back(std::bind(&RandomCircuit::set_layer_type_1, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	m_set_layer_func_vec.push_back(std::bind(&RandomCircuit::set_layer_type_2, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	m_set_layer_func_vec.push_back(std::bind(&RandomCircuit::set_layer_type_3, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	m_set_layer_func_vec.push_back(std::bind(&RandomCircuit::set_layer_type_4, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	m_set_layer_func_vec.push_back(std::bind(&RandomCircuit::set_layer_type_5, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	m_set_layer_func_vec.push_back(std::bind(&RandomCircuit::set_layer_type_6, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	m_set_layer_func_vec.push_back(std::bind(&RandomCircuit::set_layer_type_7, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	m_set_layer_func_vec.push_back(std::bind(&RandomCircuit::set_layer_type_8, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

RandomCircuit::~RandomCircuit()
{
}

int RandomCircuit::get_middle_qubit(int qbitRow, int qbitColumn)
{
	return ((int)((qbitRow * qbitColumn) / 2) - 1);
}

bool RandomCircuit::is_greater_than_middle(int target_1, int target_2, int middle)
{
	if ((target_1 > middle) && (target_2 > middle))
	{
		return false;
	}
	else if ((target_1 <= middle) && (target_2 <= middle))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool RandomCircuit::is_need_break_up(int middle,  int qbitRow, int qbitColumn, LayerInfo& layer)
{
	for (int i = 0; i < qbitColumn; i++)
	{
		for (int j = 0; j < qbitRow; j++)
		{
			switch (layer[j][i].gate_type) {
			case 1:
				if (i + 1 >= qbitColumn || j + 1 >= qbitRow) 
				{
					if (i + 1 >= qbitColumn) 
					{
						if (j + 1 >= qbitRow)
						{
						}
						else if (layer[j + 1][i].gate_type == 1)
						{
							if (is_greater_than_middle((j + i * qbitRow), (j + 1 + i * qbitRow), middle))
							{
								return true;
							}
						}
						break;
					}
					else if (j + 1 >= qbitRow) 
					{
						if (i + 1 >= qbitColumn) 
						{
						}
						else if (layer[j][i + 1].gate_type == 1) 
						{
							if (is_greater_than_middle((j + i * qbitRow), (j + (i + 1)*qbitRow), middle))
							{
								return true;
							}
						}
						break;
					}
				}
				else
				{
					if (layer[j][i + 1].gate_type == 1) 
					{
						if (is_greater_than_middle((j + i * qbitRow), (j + (i + 1)*qbitRow), middle))
						{
							return true;
						}
					}
					else if (layer[j + 1][i].gate_type == 1) 
					{
						if (is_greater_than_middle((j + i * qbitRow), (j + 1 + i * qbitRow), middle))
						{
							return true;
						}
					}
					else 
					{
					}
					break;
				}
			}
		}
	}
	return false;
}

bool RandomCircuit::set_layer_type_1(int qbitRow, int qbitColumn, LayerInfo& layer)
{
	for (int i = 0; i < qbitRow; ++i) 
	{
		for (int j = 0; j < qbitColumn; ++j)
		{
			if (i % 2 == 1) 
			{
				if (j % 4 == 1) 
				{
					layer[i][j].gate_type = 1;
				}
				else if (j % 4 == 0)
				{
					if (j != qbitColumn - 1)
					{
						layer[i][j].gate_type = 1;
					}
				}
			}
			else
			{
				if (j % 4 == 3)
				{
					layer[i][j].gate_type = 1;
				}
				else if (j % 4 == 2) 
				{
					if (j != qbitColumn - 1)
					{
						layer[i][j].gate_type = 1;
					}
				}
			}
		}
	}
	int middle_qubit = get_middle_qubit(qbitRow, qbitColumn);
	return is_need_break_up( middle_qubit, qbitRow, qbitColumn, layer);
}

bool RandomCircuit::set_layer_type_2(int qbitRow, int qbitColumn, LayerInfo& layer)
{
	for (int i = 0; i < qbitRow; ++i)
	{
		for (int j = 0; j < qbitColumn; ++j)
		{
			if (i % 2 == 0)
			{
				if (j % 4 == 1)
				{
					layer[i][j].gate_type = 1;
				}
				else if (j % 4 == 0) 
				{
					if (j != qbitColumn - 1) 
					{
						layer[i][j].gate_type = 1;
					}
				}
			}
			else 
			{
				if (j % 4 == 3)
				{
					layer[i][j].gate_type = 1;
				}
				else if (j % 4 == 2) 
				{
					if (j != qbitColumn - 1)
					{
						layer[i][j].gate_type = 1;
					}
				}
			}
		}
	}
	int middle_qubit = get_middle_qubit(qbitRow, qbitColumn);
	return is_need_break_up( middle_qubit, qbitRow, qbitColumn, layer);
}

bool RandomCircuit::set_layer_type_3(int qbitRow, int qbitColumn, LayerInfo& layer) 
{
	for (int i = 0; i < qbitRow; ++i)
	{
		for (int j = 0; j < qbitColumn; ++j)
		{
			if (j % 2 == 0)
			{
				if (i % 4 == 3)
				{
					if (i != qbitRow - 1) 
					{
						layer[i][j].gate_type = 1;
					}
				}
				else if (i % 4 == 0 && i != 0) 
				{
					layer[i][j].gate_type = 1;
				}
			}
			else 
			{
				if (i % 4 == 2) 
				{
					layer[i][j].gate_type = 1;
				}
				else if (i % 4 == 1)
				{
					if (i != qbitRow - 1)
					{
						layer[i][j].gate_type = 1;
					}
				}
			}
		}
	}
	int middle_qubit = get_middle_qubit(qbitRow, qbitColumn);
	return is_need_break_up(middle_qubit, qbitRow, qbitColumn, layer);
}

bool RandomCircuit::set_layer_type_4(int qbitRow, int qbitColumn, LayerInfo& layer)
{
	for (int i = 0; i < qbitRow; ++i)
	{
		for (int j = 0; j < qbitColumn; ++j) 
		{
			if (j % 2 == 1) 
			{
				if (i % 4 == 3)
				{
					if (i != qbitRow - 1) 
					{
						layer[i][j].gate_type = 1;
					}
				}
				else if (i % 4 == 0 && i != 0)
				{
					layer[i][j].gate_type = 1;
				}
			}
			else {
				if (i % 4 == 2)
				{
					layer[i][j].gate_type = 1;
				}
				else if (i % 4 == 1) 
				{
					if (i != qbitRow - 1)
					{
						layer[i][j].gate_type = 1;
					}
				}
			}
		}
	}
	int middle_qubit = get_middle_qubit(qbitRow, qbitColumn);
	return is_need_break_up(middle_qubit, qbitRow, qbitColumn, layer);
}

bool RandomCircuit::set_layer_type_5(int qbitRow, int qbitColumn, LayerInfo& layer) 
{
	for (int i = 0; i < qbitRow; ++i)
	{
		for (int j = 0; j < qbitColumn; ++j)
		{
			if (i % 2 == 0)
			{
				if (j % 4 == 3) 
				{
					layer[i][j].gate_type = 1;
				}
				else if (j % 4 == 0) 
				{
					if (j != 0) 
					{
						layer[i][j].gate_type = 1;
					}
				}
			}
			else 
			{
				if (j % 4 == 2)
				{
					layer[i][j].gate_type = 1;
				}
				else if (j % 4 == 1) 
				{
					if (j != qbitColumn - 1)
					{
						layer[i][j].gate_type = 1;
					}
				}
			}
		}
	}

	int middle_qubit = get_middle_qubit(qbitRow, qbitColumn);
	return is_need_break_up(middle_qubit, qbitRow, qbitColumn, layer);
}

bool RandomCircuit::set_layer_type_6(int qbitRow, int qbitColumn, LayerInfo& layer)
{
	for (int i = 0; i < qbitRow; ++i) 
	{
		for (int j = 0; j < qbitColumn; ++j) 
		{
			if (i % 2 == 1)
			{
				if (j % 4 == 3) 
				{
					layer[i][j].gate_type = 1;
				}
				else if (j % 4 == 0) 
				{
					if (j != 0) 
					{
						layer[i][j].gate_type = 1;
					}
				}
			}
			else
			{
				if (j % 4 == 2) 
				{
					layer[i][j].gate_type = 1;
				}
				else if (j % 4 == 1) 
				{
					if (j != qbitColumn - 1)
					{
						layer[i][j].gate_type = 1;
					}
				}
			}
		}
	}
	int middle_qubit = get_middle_qubit(qbitRow, qbitColumn);
	return is_need_break_up(middle_qubit, qbitRow, qbitColumn, layer);
}

bool RandomCircuit::set_layer_type_7(int qbitRow, int qbitColumn, LayerInfo& layer) 
{
	for (int i = 0; i < qbitRow; ++i)
	{
		for (int j = 0; j < qbitColumn; ++j) 
		{
			if (j % 2 == 0) 
			{
				if (i % 4 == 1)
				{
					layer[i][j].gate_type = 1;
				}
				else if (i % 4 == 0) 
				{
					if (i != qbitRow - 1)
					{
						layer[i][j].gate_type = 1;
					}
				}
			}
			else 
			{
				if (i % 4 == 3) 
				{
					layer[i][j].gate_type = 1;
				}
				else if (i % 4 == 2) 
				{
					if (i != qbitRow - 1)
					{
						layer[i][j].gate_type = 1;
					}
				}
			}
		}
	}
	int middle_qubit = get_middle_qubit(qbitRow, qbitColumn);
	return is_need_break_up(middle_qubit, qbitRow, qbitColumn, layer);
}

bool RandomCircuit::set_layer_type_8(int qbitRow, int qbitColumn, LayerInfo& layer)
{
	for (int i = 0; i < qbitRow; ++i)
	{
		for (int j = 0; j < qbitColumn; ++j)
		{
			if (j % 2 == 1)
			{
				if (i % 4 == 1)
				{
					layer[i][j].gate_type = 1;
				}
				else if (i % 4 == 0) 
				{
					if (i != qbitRow - 1) 
					{
						layer[i][j].gate_type = 1;
					}
				}
			}
			else 
			{
				if (i % 4 == 3) 
				{
					layer[i][j].gate_type = 1;
				}
				else if (i % 4 == 2)
				{
					if (i != qbitRow - 1)
					{
						layer[i][j].gate_type = 1;
					}
				}
			}
		}
	}
	int middle_qubit = get_middle_qubit(qbitRow, qbitColumn);
	return is_need_break_up(middle_qubit, qbitRow, qbitColumn, layer);
}

void RandomCircuit::set_layer_one_qubit_gate(int qbitRow, int qbitColumn, LayerInfo & layer, LayerInfo& previous_layer)
{
	srand((int)time(0));
	for (int i = 0; i < qbitRow; ++i)
	{
		for (int j = 0; j < qbitColumn; ++j) 
		{
			if (layer[i][j].gate_type == 1) 
			{
				layer[i][j].has_T = previous_layer[i][j].has_T;
				continue;
			}
			if (previous_layer[i][j].gate_type == 1)
			{
				if (previous_layer[i][j].has_T == true) 
				{
					layer[i][j].gate_type = (int)(rand() % 3) + 2;
					layer[i][j].has_T = true;
				}
				else 
				{
					layer[i][j].gate_type = 4;
					layer[i][j].has_T = true;
				}
			}
			else
			{
				layer[i][j].gate_type = 0;
				layer[i][j].has_T = previous_layer[i][j].has_T;
			}
		}
	}
}

void RandomCircuit::set_all_hadamard(int qbitRow, int qbitColumn, LayerInfo& layer)
{
	for (int i = 0; i < qbitRow; ++i) 
	{
		for (int j = 0; j < qbitColumn; ++j)
		{
			layer[i][j].gate_type = 5;
		}
	}
}

void  RandomCircuit::create_one_layer(int qbitRow, int qbitColumn, LayerInfo &grid)
{
	for (int i = 0; i < qbitRow; ++i) 
	{
		vector<QubitInformation > temp_row;
		for (int j = 0; j < qbitColumn; ++j)
		{
			QubitInformation temp;
			temp.x = i;
			temp.y = j;
			temp.gate_type = 0;
			temp.has_T = false;
			temp_row.push_back(temp);
		}
		grid.push_back(temp_row);
	}
}

void RandomCircuit::generate_circuit_info(int qbitRow, int qbitColumn, int depth, vector<LayerInfo>& qubit_information_vector)
{
	LayerInfo layer0;
	create_one_layer(qbitRow, qbitColumn, layer0);
	set_all_hadamard(qbitRow, qbitColumn, layer0);
	qubit_information_vector.push_back(layer0);

	int num = 0;
	auto iter2 = (m_set_layer_func_vec.end() - 1);
	for (auto iter = m_set_layer_func_vec.begin(); iter != iter2;)
	{
		LayerInfo layer;
		create_one_layer(qbitRow, qbitColumn, layer);
		auto result = (*iter)(qbitRow, qbitColumn, layer);
		if (result)
		{
			auto temp = (*iter);
			(*iter) = (*iter2);
			(*iter2) = temp;
			iter2--;
			num++;
		}
		else
		{
			iter++;
		}
	}

	for (int i = 0; i < depth; i++) 
	{
		LayerInfo layer;
		create_one_layer(qbitRow, qbitColumn, layer);
		int j = i % 8;
		m_set_layer_func_vec[j](qbitRow, qbitColumn, layer);
		set_layer_one_qubit_gate(qbitRow, qbitColumn, layer, qubit_information_vector[qubit_information_vector.size() - 1]);
		qubit_information_vector.push_back(layer);
	}
}

static std::string _tostring(const float val)
{
	std::ostringstream out;
	unsigned int precision = 8;  
	out.precision(precision);
	out << val;
	return out.str();
}

void RandomCircuit::generate_random_circuit(vector<LayerInfo> circuit, int qbitRow, int qbitColumn)
{
	stringstream str_originir;
	str_originir << "QINIT " << (qbitRow*qbitColumn) << '\n';
	str_originir << "CREG " << (qbitRow*qbitColumn) << '\n';

	m_qv = m_qvm->qAllocMany(qbitRow * qbitColumn);

	for (auto & layer : circuit)
	{
		for (int i = 0; i < qbitColumn; i++)
		{
			for (int j = 0; j < qbitRow; j++)
			{
				switch (layer[j][i].gate_type)
				{
				case 1:
					if (i + 1 >= qbitColumn || j + 1 >= qbitRow)
					{
						if (i + 1 >= qbitColumn)
						{
							if (j + 1 >= qbitRow)
							{
							}
							else if (layer[j + 1][i].gate_type == 1)
							{
								str_originir << "CR " 
									<< "q["<< (j + i * qbitRow) << "]"  << "," 
									<< "q[" << (j + 1 + i * qbitRow) << "]" << ","
									<< "("  << _tostring (PI) << ")"<< "\n";
								m_prog << CR(m_qv[j + i * qbitRow], m_qv[j + 1 + i * qbitRow], PI);
							}
							break;
						}
						else if (j + 1 >= qbitRow)
						{
							if (i + 1 >= qbitColumn)
							{
							}
							else if (layer[j][i + 1].gate_type == 1)
							{
								str_originir << "CR "
									<< "q[" << (j + i * qbitRow) << "]" << ","
									<< "q[" << (j + (i + 1)*qbitRow) << "]" << ","
									<< "(" << _tostring(PI) << ")" << "\n";
								m_prog << CR(m_qv[j + i * qbitRow], m_qv[j + (i + 1)*qbitRow], PI);
							}
							break;
						}
					}
					else
					{
						if (layer[j][i + 1].gate_type == 1)
						{
							str_originir << "CR "
								<< "q[" << (j + i * qbitRow) << "]" << ","
								<< "q[" << (j + (i + 1)*qbitRow) << "]" << ","
								<< "(" << _tostring(PI) << ")" << "\n";
							m_prog << CR(m_qv[j + i * qbitRow], m_qv[j + (i + 1)*qbitRow], PI);
						}
						else if (layer[j + 1][i].gate_type == 1)
						{
							str_originir << "CR "
								<< "q[" << (j + i * qbitRow) << "]" << ","
								<< "q[" << (j + 1 + i * qbitRow) << "]" << ","
								<< "(" << _tostring(PI) << ")" << "\n";
							m_prog << CR(m_qv[j + i * qbitRow], m_qv[j + 1 + i * qbitRow], PI);
						}
						else
						{
						}
						break;
					}
				case 2:
				{
					str_originir << "RX " << "q[" << (j + i * qbitRow) << "]" << "," << "(" << _tostring(PI/2) << ")" << "\n";
					m_prog << RX(m_qv[j + i * qbitRow], PI / 2);
				}
				break;
				case 3:
				{
					str_originir << "RY " << "q[" << (j + i * qbitRow) << "]" << "," << "(" << _tostring(PI / 2) << ")" << "\n";
					m_prog << RY(m_qv[j + i * qbitRow], PI / 2);
				}
				break;
				case 4:
				{
					str_originir << "RZ " << "q[" << (j + i * qbitRow) << "]" << "," << "(" << _tostring(PI / 4) << ")" << "\n";
					m_prog << RZ(m_qv[j + i * qbitRow], PI / 4);
				}
				break;
				case 5:
				{
					str_originir << "H " << "q[" << (j + i * qbitRow) << "]" << "\n";
					m_prog << H(m_qv[j + i * qbitRow]);
				}
				break;
				default:
					break;
				}
			}
		}
	}
	m_originr = str_originir.str();
}


std::string RandomCircuit::get_random_originir()
{
	if (m_originr.empty())
	{
		QCERR("m_originr is null!");
		throw invalid_argument("m_originr is null!");
	}
	return m_originr;
}

QProg RandomCircuit::get_random_qprog()
{
	return m_prog;
}

void RandomCircuit::random_circuit(int qbitRow, int qbitColumn, int depth)
{
	vector<LayerInfo> circuit_info;
	generate_circuit_info(qbitRow, qbitColumn, depth, circuit_info);
	generate_random_circuit(circuit_info, qbitRow, qbitColumn);
}

QProg QPanda::random_qprog(int qubitRow, int qubitCol, int depth, QuantumMachine *qvm, QVec &qv)
{
	if (qubitRow <= 0 || qubitCol <= 0 || depth <= 0)
	{
		QCERR("parameter error!");
		throw invalid_argument("parameter error!");
	}
	RandomCircuit random_cir(qvm, qv);
	
	random_cir.random_circuit(qubitRow, qubitCol, depth);
	return random_cir.get_random_qprog();
}

std::string QPanda::random_originir(int qubitRow, int qubitCol, int depth, QuantumMachine *qvm, QVec &qv)
{
	if (qubitRow <= 0 || qubitCol <= 0 || depth <= 0)
	{
		QCERR("parameter error!");
		throw invalid_argument("parameter error!");
	}
	RandomCircuit random_cir(qvm, qv);

	random_cir.random_circuit(qubitRow, qubitCol, depth);
	return random_cir.get_random_originir();
}


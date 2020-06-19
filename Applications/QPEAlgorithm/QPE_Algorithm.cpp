/*
Copyright (c) 2017-2020 Origin Quantum Computing. All Right Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "Core/Core.h"
using namespace std;
using namespace QPanda;
#define QGEN function<QCircuit (vector<Qubit*>)> 

QCircuit QFT(vector<Qubit*> qvec)
{
    QCircuit  qft = CreateEmptyCircuit();
    for(auto i=0;i<qvec.size();i++)
    {
        qft << H(qvec[qvec.size()- 1 - i]);
        for (auto j = i+1; j < qvec.size(); j++)
        {
            qft << CR(qvec[qvec.size() - 1 - j],
                qvec[qvec.size() - 1 - i], 2*PI / (1 << (j - i + 1)));
        }
    }
    return qft;
}
QCircuit QFTdagger(vector<Qubit*> qvec)
{
    QCircuit  qft = QFT(qvec);
    qft.setDagger(true);
    return qft;
}

QCircuit   unitary(vector<Qubit*> qvec)
{
    QCircuit  circuit = CreateEmptyCircuit();
    circuit << RX(qvec[0],-PI);
    return circuit;
}
QCircuit   Hadamard(vector<Qubit*> qvec)
{
    QCircuit  circuit = CreateEmptyCircuit();
    for (auto iter = qvec.begin(); iter != qvec.end(); iter++)
    {
        circuit << H(*iter);
    }
    return circuit;
}

// U is generated by qc,
// output U^(2^min)
QCircuit   unitarypower(vector<Qubit*> qvec, size_t min, QGEN qc)
{
    QCircuit  circuit = CreateEmptyCircuit();
    for (auto i = 0; i < (1 << min); i++)
    {
        circuit << qc(qvec);
    }
    return circuit;
}
QCircuit   controlUnitaryPower(vector<Qubit*> qvec,Qubit *ControlQubit, size_t min, QGEN qc)
{
    QCircuit  qCircuit = unitarypower(qvec, min, qc);
    vector<Qubit*> cControlQubit = { ControlQubit };
    qCircuit.setControl(cControlQubit);
    return qCircuit;
}

QCircuit QPE(vector<Qubit*> controlqvec, vector<Qubit *> targetqvec, QGEN qc)
{
    QCircuit  circuit = CreateEmptyCircuit();
    for (auto i = 0; i < controlqvec.size(); i++)
    {
        circuit << H(controlqvec[i]);
    }
    vector<Qubit*> controlqubit;
    for (auto i=0;i<controlqvec.size();i++)
    {
        circuit << 
            controlUnitaryPower(targetqvec, controlqvec[controlqvec.size() - 1 - i], i, qc);
    }
    circuit << QFTdagger(controlqvec);
    return circuit;
}

int main()
{
    init(QMachineType::CPU);
    int qubit_number = 2;
    int cbitnum = 2;
    vector<Qubit*> cqv = qAllocMany(qubit_number);      //control
    vector<Qubit*> tqv = qAllocMany(1);
    vector<ClassicalCondition> cv = cAllocMany(cbitnum);
    auto qpeProg = CreateEmptyQProg();
    qpeProg << H(tqv[0]);
    qpeProg << QPE(cqv, tqv, unitary);
    qpeProg << Measure(cqv[0], cv[0]) << Measure(cqv[1], cv[1]);
    directlyRun(qpeProg);
    cout << "c0: " << cv[0].get_val()<< endl;
    cout << "c1: " << cv[1].get_val() << endl;
    finalize();
}



/*
Copyright (c) 2017-2018 Origin Quantum Computing. All Right Reserved.

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

#include "HHL_Algorithm.h"

void HHL_Algorithm()
{
    map<string, bool> temp;
    int x0 = 0;
    int x1 = 0;
    for (size_t i = 0; i < 1000;i++)
    {
        temp = hhlalgorithm();
        if (temp["c0"])
        {
            if (temp["c1"])
            {
                x1++;
            }
            else
            {
                x0++;
            }
        }
    }
    int sum = x0 + x1;
    cout << "prob0:" << x0*1.0/sum << endl;
    cout << "prob1:" << x1*1.0/sum << endl;
}

map<string, bool> hhlalgorithm()
{
    init();
    int qubitnum = 4;
    vector<Qubit*> qv;
    for (size_t i = 0; i < qubitnum; i++)
    {
        qv.push_back(qAlloc());
    }
    vector<CBit*> cv;
    int cbitnum = 2;
    for (size_t i = 0; i < cbitnum; i++)
    {
        cv.push_back(cAlloc());
    }
    auto &hhlprog =CreateEmptyQProg(); 
    hhlprog << RY(qv[3], PI / 2);       //  change vecotr b in equation Ax=b
    hhlprog << hhl(qv, cv);
    load(hhlprog);
    run();
    auto resultMap = getResultMap();
    finalize();
    return resultMap;
}


QuantumProgram& hhl(vector<Qubit*> qVec, vector<CBit*> cVec)
{
    ClassicalCondition cc0=bind_a_cbit(cVec[0]);

	// meaningless sentence
    QuantumCircuit & ifcircuit = CreateEmptyCircuit();

    QuantumCircuit & PSEcircuit = hhlPse(qVec);//PSE
    QuantumCircuit & CRot = CRotate(qVec);//control-lambda
    QuantumCircuit & PSEcircuitdag = hhlPse(qVec);
    //hhl circuit
    QuantumProgram & PSEdagger = CreateEmptyQProg();

    PSEdagger << PSEcircuitdag.dagger() << Measure(qVec[3], cVec[1]);
    QuantumIf  ifnode = CreateIfProg(cc0, &PSEdagger);
    QuantumProgram & hhlProg = CreateEmptyQProg();
    //hhlProg << PSEcircuit <<CRot<<  Measure(qVec[0], cVec[0])<<ifnode;
    hhlProg << PSEcircuit << CRot << Measure(qVec[0], cVec[0]) << ifnode;
    return hhlProg;
}
QuantumCircuit& hhlPse(vector<Qubit*> qVec)
{
    QuantumCircuit & PSEcircuit = CreateEmptyCircuit();
    PSEcircuit << H(qVec[1]) << H(qVec[2]) << RZ(qVec[2], 0.75*PI);
    QuantumGate & gat1 = QDouble(PI, 1.5*PI, -0.5*PI, PI / 2, qVec[2], qVec[3]);
    QuantumGate  & gat2 = QDouble(PI, 1.5*PI, -PI, PI / 2, qVec[1], qVec[3]);
    PSEcircuit << gat1 << RZ(qVec[1], 1.5*PI) << gat2;
    PSEcircuit << CNOT(qVec[1], qVec[2]) << CNOT(qVec[2], qVec[1]) << CNOT(qVec[1], qVec[2]);
    //PSEcircuit << gat1 << RZ(q1, 1.5*PI)<<gat2 ;
    QuantumGate & gat3 = QDouble(-0.25*PI, -0.5*PI, 0, 0, qVec[2], qVec[1]);
    PSEcircuit << H(qVec[2]) << gat3 << H(qVec[1]);     //PSE over
    return PSEcircuit;
}
QuantumCircuit& CRotate(vector<Qubit*> qVec)
{
    QuantumCircuit & CRot = CreateEmptyCircuit();
    vector<Qubit *> controlVector;
    controlVector.push_back(qVec[1]);
    controlVector.push_back(qVec[2]);
    QuantumGate & gat4 = RY(qVec[0], PI);
    gat4.setControl(controlVector);
    QuantumGate & gat5 = RY(qVec[0], PI / 3);
    gat5.setControl(controlVector);
    QuantumGate & gat6 = RY(qVec[0], 0.679673818908);  //arcsin(1/3)
    gat6.setControl(controlVector);
    CRot << RX(qVec[1]) << gat4 << RX(qVec[1]) << RX(qVec[2]) << gat5 << RX(qVec[2]) << gat6;
    //CRot << RX(qVec[1]) << gat4 << RX(qVec[1]);
    return CRot;
}

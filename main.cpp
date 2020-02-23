  // Copyright [2018] Cruceru Vlad
#include <iostream>
#include <fstream>
#include <cstdio>
#include "Dequeue.h"
#include "Stack.h"

std::ofstream outfile;
std::ifstream infile;

  // functia UNDO
template < typename T >
void UNDO(Dequeue <T> robot[] , Stack <T> *last_executed ,
Stack <T> *undo , int **map)
{
    Data dataout;
    int index;
    dataout = (*undo).pop();  // scot din stiva undo comanda
    index = dataout.id;
    // daca comanda are prioritate 0 sau 1 inseamna ca e de tipul ADD
    // si o scot din coada robotului cu id-ul index
    if ( dataout.priority == 0)
        robot[index].removeFirst();
    if ( dataout.priority == 1)
        robot[index].removeLast();
    // daca are prioritatea -1 inseamna ca e EXECUTE
    if ( dataout.priority == -1)
        {
	    // resetez prioritatea si timpul
            dataout.priority = 0;
            dataout.time = 0;
	    // verific ce tip de comanda e pt a prelucra Nr de cutii
            if (dataout.CmdType == "GET")
            {
                robot[ index ].NrBoxes -= dataout.NrBoxes;
                map[ dataout.x ][ dataout.y ] += dataout.NrBoxes;
            }
            if (dataout.CmdType == "DROP")
            {
                robot[ index ].NrBoxes += dataout.NrBoxes;
                map[ dataout.x ][ dataout.y ] -= dataout.NrBoxes;
            }
	    // adaug comanda inapoi in coada robotului cu id-ul index
            robot[ index ].addFirst(dataout);
	    // scot comanda din stiva last_executed
            (*last_executed).pop();
        }
}

  // functie pentru comanda HOW_MUCH_TIME
template < typename T >
void TIME(Stack <T> *last_executed)
{
    Data dataout;
    dataout = (*last_executed).peek();
    outfile << "HOW_MUCH_TIME: " << dataout.time << std::endl;
}

  // functie pentru comanda LAST_EXECUTED
template < typename T >
void LAST_EXECUTED(Stack<T> *last_executed)
{
    Data dataout;
    dataout = (*last_executed).peek();
    outfile << "LAST_EXECUTED_COMMAND: " << dataout.id << ": " <<
    dataout.CmdType << " " << dataout.x << " " << dataout.y << " " <<
    dataout.NrBoxes << std::endl;
}

  // functie pentru adaugare in coada a comenzii ADD
template < typename T >
void ADD(Dequeue <T> robot[] , Data datain){
    int index = datain.id;
    // adauga in coada robotului cu id-ul index in functie de prioritate
    if (datain.priority == 0)
        robot[ index ].addFirst(datain);
    if (datain.priority == 1)
        robot[ index ].addLast(datain);
}

  // functie pt EXECUTE
template < typename T >
void EXECUTE(Dequeue <T> robot[] , Data datain , int **map ,
Stack <T> *last_executed , Stack <T> *undo){
    int index = datain.id;
    // scot informatia comenzii din coada si ii trec prioritatea pe -1
    Data dataout = robot[ index ].removeFirst();
    dataout.priority = -1;
    // in functie de tipul comenzii prelucrez Nr de cutii
    if (dataout.CmdType == "GET")
        {
            if (dataout.NrBoxes > map[dataout.x][dataout.y])
                dataout.NrBoxes = map[dataout.x][dataout.y];
            robot[index].NrBoxes += dataout.NrBoxes;
            map[dataout.x][dataout.y] -= dataout.NrBoxes;
        }
    if (dataout.CmdType == "DROP")
        {
            if (dataout.NrBoxes > robot[index].NrBoxes)
                dataout.NrBoxes = robot[index].NrBoxes;
            robot[index].NrBoxes -= dataout.NrBoxes;
            map[dataout.x][dataout.y] += dataout.NrBoxes;
        }
  // adaug comanda executata atat in stiva last_executed cat si in cea de undo
    (*last_executed).push(dataout);
    (*undo).push(dataout);
}

  // o functie de citire
Data read()
        {
    Data datain;
    // citesc tipul comenzii
    infile >> datain.CmdType;
    // daca comanda e asa,citesc si id,x,y,NrBoxes,priority
    if (datain.CmdType == "ADD_GET_BOX" || datain.CmdType == "ADD_DROP_BOX")
        infile >> datain.id >> datain.x >> datain.y >> datain.NrBoxes >>
datain.priority;
    // daca comanda e asa,citesc doar id
    if (datain.CmdType == "EXECUTE" || datain.CmdType == "PRINT_COMMANDS" ||
datain.CmdType == "HOW_MANY_BOXES")
        infile >> datain.id;
    if (datain.CmdType == "ADD_GET_BOX")
        datain.CmdType = "GET";
    if (datain.CmdType == "ADD_DROP_BOX")
        datain.CmdType = "DROP";
    return datain;
    }
int main()
{
    outfile.open("robots.out");
    infile.open("robots.in");
    // variabilele de care am nevoie
    int **map , n , lin , col , i , j;
    Data datain , dataout;
    Dequeue <Data> *robot;  // cozile robotilor
    Stack <Data> undo , last_executed;  // stivele de undo si last_executed
    // citirea variabilelor si alocarea memoriei
    infile >> n >> lin >> col;
    robot = new Dequeue <Data> [n];
    map = new int*[lin];
    for ( i = 0 ; i < lin ; i++)
        map[i] = new int[col];
    for ( i = 0 ; i < lin ; i++)
        for ( j = 0 ; j < col ; j++)
                infile >> map[i][j];

    while (true)
    {
        // pt fiecare comanda citita execut timer pentru fiecare coada
	for ( i = 0 ; i < n ; i++)
            robot[ i ].timer();
        datain =  read();
	// as fi pus asta in conditia while dar imi citeste de 2 ori ultima
	// linie, nu stiu de ce
	if (infile.eof()){
                      break;
	}
	// daca comanda e ADD, adauga in coada robotului si in stiva undo
        if (datain.CmdType == "GET" || datain.CmdType == "DROP")
                {
                 ADD(robot , datain);
                 undo.push(datain);
                }
	// daca e PRINT afiseaza comenzile din coada robotului
	// operatorul e supraincarcat (vezi Dequeue.h)
        if (datain.CmdType == "PRINT_COMMANDS")
            {
                outfile << "PRINT_COMMANDS: ";
                outfile << robot[datain.id] << std::endl;
            }
	// daca e HOW_MANY afiseaza variabila NrBoxes a robotului id
        if (datain.CmdType == "HOW_MANY_BOXES")
         outfile << "HOW_MANY_BOXES: " << robot[datain.id].NrBoxes <<std::endl;
        // daca e EXECUTE verifica daca robotul are comanda in coada si
	// apeleaza functia EXECUTE
        if (datain.CmdType == "EXECUTE")
                {
                    if (robot[datain.id].size() == 0)
                      outfile << "EXECUTE: No command to execute" << std::endl;
                    else
                        EXECUTE(robot , datain , map , &last_executed , &undo);
                }
	// daca e LAST_EXECUTED verifica daca s-a executat vreo comanda si
	// apeleaza functia LAST_EXECUTED
        if (datain.CmdType == "LAST_EXECUTED_COMMAND")
            {
                if (last_executed.size() == 0)
                 	outfile << "LAST_EXECUTED_COMMAND: No command was executed"
			<< std::endl;
                else
                    LAST_EXECUTED(&last_executed);
            }
	// daca e HOW_MUCH_TIME verifica daca s-a executat vreo comanda si
        // apeleaza functia TIME
        if (datain.CmdType == "HOW_MUCH_TIME")
            {
                if (last_executed.size() == 0)
                    outfile << "HOW_MUCH_TIME: No command was executed"
		    << std::endl;
                else
                    TIME(&last_executed);
            }
	// daca e UNDO verifica daca stiva undo e goala si apeleaza fct UNDO
        if (datain.CmdType == "UNDO")
            {
                if (undo.size() == 0)
                    outfile << "UNDO: No History" << std::endl;
                else
                    UNDO(robot , &last_executed , &undo , map);
            }
    }
    // eliberez memoria
    for ( i = 0 ; i < lin ; i++)
        delete[] map[i];
    delete[] map;
	delete[] robot;

outfile.close();
infile.close();

    return 0;
}

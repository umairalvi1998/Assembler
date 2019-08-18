#include<iostream>
#include<iomanip>
using namespace std;
const int size=100/*size of memory(const indicate size cannot be change)
*/,MAX_WORD=9999,MIN_WORD=-9999;/*limit of the word cannot be greater and less than this*/
const long sentinel=-99999;//user will enter this value to get out of the execution loop
enum commands { READ=10,WRITE,LOAD=20,STORE,ADD=30,SUBTRACT,DIVIDE,MULTIPLY,BRANCH=40,BRANCHNEG,BRANCHZERO,HALT};
void load(int * const);//function prototypes
void execute(int * const,int * const,int * const,int * const,int * const,int *const);
void dump(const int * const,int,int,int,int,int);
bool validWord(int);
int main()
{
  int memory[size]={0}/*all the array elements are initialized with 0*/,accumulator=0,instructioncounter=0,opcode=0,operand=0,instructionregister=0;
  load(memory);//passing array to the function load so that instructions can be loaded into memory
  execute(memory,&accumulator,&instructioncounter,&instructionregister,&opcode,&operand);//this function is responsible for the execution of the instructions
  dump(memory,accumulator,instructioncounter,instructionregister,opcode,operand);//at the end of the program it shows the memory contents after the execution
  return 1;
  }


  void load(int * const loadMemory)//the purpose of this funcn is to enter the instructions by the user and then load those instr
  //uctions into memory
  {
    long instruction;//instruction is  4-digit number in which first2 digits represent the opcode and other two represents the memory location where operand is stored
    int i=0;
    cout<<"***           Welcome to alvitron             ***\n"
        <<"*** please enter your program one instruction ***\n"
        <<"*** (or data word) at a time.I will type the  ***\n"
        <<"*** location number and a question mark (?).  ***\n"
        <<"*** you then type the word for that location. ***\n"
        <<"*** type the sentinel -99999 to stop entering ***\n"
        <<"*** your program.                             ***\n"<< "00 ? ";
    cin>>instruction;

    while(instruction != sentinel)//this will take the instructions as long as they are valid i.e when user enters the value
//-99999 or 99999 for stop entering the instructions.
    {
      if(!validWord(instruction))
         cout<<"number out of range .Please enter again.\n";
      else
        loadMemory[i++]=instruction;//the instruction which was entered by the user is loaded into the memory i.e array.
        cout<<setw(2)<<setfill('0')<<i<<" ? ";//shows the memory locations
        cin>>instruction;
         }
    }
    bool validWord(int word)
    {
     return word >= MIN_WORD && word<=MAX_WORD;
     }
void execute(int * const memory,int * const acPtr,int * const icPtr,int * const irPtr,int * const opCodePtr,int * const opPtr)
{
    bool fatal=false;
    int temp;
    const char *messages[]={"Accumulator overflow          ***"
                            "Attempt to divide by Zero     ***"
                            "Invalid opcode detected       ***"},
          *termstring="\n*** Alvitron execution abnormally terminated ***",
          *fatalString="*** FATAL ERROR: ";
     cout<<"\n*********************START ALVITRON EXECUTION*********************\n\n";

     do
     {
         *irPtr=memory[*icPtr];//irPtr is instruction register which stores the current instruction which is to be executed
         *opCodePtr=*irPtr/100;//opcode is fetched from the instruction i.e the first two digits
         *opPtr=*irPtr%100;//operand(memory address)is fetched from the instruction i.e the last two digits

         switch(*opCodePtr)
         {
         case READ:
            cout<<"enter an integer: ";
            cin>>temp;
            while(!validWord(temp))
            {
                cout<<"Number out of range please enter again: ";
                cin>>temp;
            }
            memory[*opPtr]=temp;//the value entered by the user is stored in the memory location specified in the instruction
            ++(*icPtr);//instruction counter keeps track of the memory
            break;
         case WRITE:
            cout<<"Contents of "<<setw(2)<<setfill('0')<<*opPtr<<": "<<memory[*opPtr]<<'\n';
            ++(*icPtr);
            break;
         case LOAD:
            *acPtr=memory[*opPtr];
            ++(*icPtr);
            break;
         case STORE:
            memory[*opPtr]=*acPtr;
            ++(*icPtr);
            break;
         case ADD:
             temp=*acPtr+memory[*opPtr];
             if(!validWord(temp))
             {
                 cout<<"fatal string"<<messages[0]<<termstring<<'\n';
                 fatal=true;
             }
             else
             {
                 *acPtr=temp;
                 ++(*icPtr);
             }
             break;
         case SUBTRACT:
            temp=*acPtr-memory[*opPtr];
            if(!validWord(temp))
             {
                 cout<<"fatal string"<<messages[0]<<termstring<<'\n';
                 fatal=true;
             }
             else
             {
                 *acPtr=temp;
                 ++(*icPtr);
             }
             break;


         case DIVIDE:
            if(memory[*opPtr]==0)
            {
                cout<<fatalString<<messages[1]<<termstring<<'\n';
                fatal=true;
            }
            else
            {
                *acPtr /=memory[*opPtr];
                ++(*icPtr);
            }
            break;
         case MULTIPLY:
            temp=*acPtr * memory[*opPtr];
            if(!validWord(temp))
            {
                cout<<fatalString<<messages[0]<<termstring<<'\n';
                fatal=true;
            }
            else
            {
                *acPtr=temp;
                ++(*icPtr);
            }
            break;
         case BRANCH:
            *icPtr=*opPtr;
            break;
         case BRANCHNEG:
            *acPtr < 0 ? *icPtr=*opPtr : ++(*icPtr);
            break;
         case BRANCHZERO:
            *acPtr==0 ? *icPtr=*opPtr : ++(*icPtr);
            break;
         case HALT:
            cout<<"***ALVITRON execution terminated***\n";
            break;
         default:
            cout<<fatalString<<messages[2]<<termstring<<'\n';
            fatal=true;
            break;

            }
     }while(*opCodePtr!=HALT && !fatal);

     cout<<"****************************End EXECUTION****************************\n";

}
void dump(const int * const memory,int accumulator,int instructioncounter,int instructionRegister,int operationCode,int operand)
{
    void output(const char * const,int,int,bool);
    cout<<"\nREGISTERS:\n";
    output("accumulator",5,accumulator,true);
    output("instructionCounter",2,instructioncounter,false);
    output("instructionRegister",5,instructionRegister,true);
    output("operationCode",2,operationCode,false);
    output("operand",2,operand,false);
    cout<<"\n\nMEMORY:\n";

    int i=0;
    cout<<setfill(' ')<<setw(3)<<' ';
    for( ;i<=9;++i)
        cout<<setw(5)<<i<<' ';
    for(i=0;i<size;++i)
    {
        if(i%10==0)
            cout<<'\n'<<setw(2)<<i<<' ';

        cout<<setiosflags(ios::internal | ios::showpos)<<setw(5)<<setfill('0')<<memory[i]<<' '<<resetiosflags(ios::internal | ios::showpos);
    }
    cout<<endl;
    }
void output(const char * const sPtr,int width,int value,bool sign)
{
    cout<<setfill( ' ' )<<left<<setw(20)<<sPtr<<' ';
    if(sign)
        cout<<setiosflags(ios::showpos | ios::internal);
    cout<<resetiosflags(ios::left)<<setfill('0');
    if(width==5)
        cout<<setw(width)<<value<<'\n';
    else
        cout<<setfill(' ')<<setw(3)<<' '<<setw(width)<<setfill('0')<<value<<'\n';
    if(sign)
        cout<<resetiosflags(ios::showpos | ios::internal);
        }


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>

void exit_eof(int * c, FILE* gbfile, FILE* asmfile);

int main(int argc, char ** argv)
{
    char * filename = NULL;
    ssize_t rc = -1;
    if (argc < 2)
    {
        printf("Please enter .gb file:\n");
        size_t size = -1;
        rc = getline(&filename, &size, stdin);
        char * eol = filename + rc - 1;
        while (isspace( *eol)) 
        {
            *eol = '\0';
            eol = eol - 1;
        }
    } else {
        filename = argv[1];
    }
    
    // Verify filename
    if (strcmp(".gb",filename + strlen(filename) - 3) != 0)
    {
        printf("File must be of type .gb\n");
        return -1;
    }

    // Open gb file
    FILE * gbfile= fopen(filename, "r");
    if (argc < 2) {
        free(filename);
    }
    if (gbfile == NULL)
    {
        perror("Failed to open .gb file\n");    
        fclose(gbfile);
        return -1;
    }

    // Get output filename and open output file
    filename = NULL;
    FILE * asmfile = stdout;
    if (argc > 2)
    {
        filename = argv[2];
        asmfile = fopen(filename, "w");
    }
    
    if (asmfile != stdout && asmfile == NULL)
    {
        perror("Failed to open output file\n");    
        fclose(asmfile);
        fclose(gbfile);
        return -1;
    }
 
    // Ready to start disassembly
    const char * inst0 = NULL;
    const char * inst1 = NULL;
    int c = getc(gbfile);
    while(c != EOF)
    {
        inst0 = NULL;
        inst1 = NULL;
        unsigned char byte = (char) c;
        unsigned char nibble0 = c & 0b00001111;
        unsigned char nibble1 = ((c & 0b11110000) >> 4) & (0b00001111);
        bool cb = false;
        bool d8 = false, d16 = false, a8 = false, a16 = false, r8 = false;
        bool offset = false;
        switch(nibble1) {
            case 0x0 :
                switch(nibble0) {
                    case 0x0 :
                        inst0 = "NOP";
                        break;
                    case 0x1 :
                        inst0 = "LD BC,";
                        d16 = true;
                        break;
                    case 0x2 :
                        inst0 = "LD (BC),A";
                        break;
                    case 0x3 :
                        inst0 = "INC BC";
                        break;
                    case 0x4 :
                        inst0 = "INC B";
                        break;
                    case 0x5 :
                        inst0 = "DEC B";
                        break;
                    case 0x6 :
                        inst0 = "LD B,";
                        d8 = true;
                        break;
                    case 0x7 :
                        inst0 = "RLCA";
                        break;
                    case 0x8 :
                        inst0 = "LD (";
                        inst1 = "),sp";
                        a16 = true;
                        break;
                    case 0x9 :
                        inst0 = "ADD HL, BC";
                        break;
                    case 0xa :
                        inst0 = "LD A, (BC)";
                        break;
                    case 0xb :
                        inst0 = "DEC BC"; 
                        break;
                    case 0xc :
                        inst0 = "INC C";
                        break;
                    case 0xd :
                        inst0 = "DEC C";
                        break;
                    case 0xe :
                        inst0 = "LD C,";
                        d8 = true;
                        break;
                    case 0xf :
                        inst0 = "RRCA";
                        break;
                }
                break;
            case 0x1 :
                switch(nibble0) {
                    case 0x0 :
                        inst0 = "STOP 0";
                        break;
                    case 0x1 :
                        inst0 = "LD DE,";
                        d16 = true;
                        break;
                    case 0x2 :
                        inst0 = "LD (DE),A";
                        break;
                    case 0x3 :
                        inst0 = "INC DE";
                        break;
                    case 0x4 :
                        inst0 = "INC D";
                        break;
                    case 0x5 :
                        inst0 = "DEC D";
                        break;
                    case 0x6 :
                        inst0 = "LD D,";
                        d8 = true;
                        break;
                    case 0x7 :
                        inst0 = "RLA";
                        break;
                    case 0x8 :
                        inst0 = "JR ";
                        r8 = true;
                        break;
                    case 0x9 :
                        inst0 = "ADD HL,DE";
                        break;
                    case 0xa :
                        inst0 = "LD A,(DE)";
                        break;
                    case 0xb :
                        inst0 = "DEC DE";
                        break;
                    case 0xc :
                        inst0 = "INC E";
                        break;
                    case 0xd :
                        inst0 = "DEC E";
                        break;
                    case 0xe :
                        inst0 = "LD E,";
                        d8 = true;
                        break;
                    case 0xf :
                        inst0 = "RRA";
                        break;
                }
                break;
            case 0x2 :
                switch(nibble0) {
                    case 0x0 :
                        inst0 = "JR NZ,";
                        r8 = true;
                        break;
                    case 0x1 :
                        inst0 = "LD HL,";
                        d16 = true;
                        break;
                    case 0x2 :
                        inst0 = "LD (HL+),A";
                        break;
                    case 0x3 :
                        inst0 = "INC HL";
                        break;
                    case 0x4 :
                        inst0 = "INC H";
                        break;
                    case 0x5 :
                        inst0 = "DEC H";
                        break;
                    case 0x6 :
                        inst0 = "LD H,";
                        d8 = true;
                        break;
                    case 0x7 :
                        inst0 = "DAA";
                        break;
                    case 0x8 :
                        inst0 = "JR Z,";
                        r8 = true;
                        break;
                    case 0x9 :
                        inst0 = "ADD HL,HL";
                        break;
                    case 0xa :
                        inst0 = "LD A,(HL+)";
                        break;
                    case 0xb :
                        inst0 = "DEC HL";
                        break;
                    case 0xc :
                        inst0 = "INC L";
                        break;
                    case 0xd :
                        inst0 = "DEC L";
                        break;
                    case 0xe :
                        inst0 = "LD L,";
                        d8 = true;
                        break;
                    case 0xf :
                        inst0 = "CPL";
                        break;
                }
                break;
            case 0x3 :
                switch(nibble0) {
                    case 0x0 :
                        inst0 = "JR NC,";
                        r8 = true;
                        break;
                    case 0x1 :
                        inst0 = "LD SP,";
                        d16 = true;
                        break;
                    case 0x2 :
                        inst0 = "LD (HL-),A";
                        break;
                    case 0x3 :
                        inst0 = "INC SP";
                        break;
                    case 0x4 :
                        inst0 = "INC (HL)";
                        break;
                    case 0x5 :
                        inst0 = "DEC (HL)";
                        break;
                    case 0x6 :
                        inst0 = "LD (HL),";
                        d8 = true;
                        break;
                    case 0x7 :
                        inst0 = "SCF";
                        break;
                    case 0x8 :
                        inst0 = "JR C,";
                        r8 = true;
                        break;
                    case 0x9 :
                        inst0 = "ADD HL,SP";
                        break;
                    case 0xa :
                        inst0 = "LD A,(HL-)";
                        break;
                    case 0xb :
                        inst0 = "DEC SP";
                        break;
                    case 0xc :
                        inst0 = "INC A";
                        break;
                    case 0xd :
                        inst0 = "DEC A";
                        break;
                    case 0xe :
                        inst0 = "LD A,";
                        d8 = true;
                        break;
                    case 0xf :
                        inst0 = "CCF";
                        break;
                }
                break;
            case 0x4 :
                switch(nibble0) {
                    case 0x0 :
                        inst0 = "LD B,B";
                        break;
                    case 0x1 :
                        inst0 = "LD B,C";
                        break;
                    case 0x2 :
                        inst0 = "LD B,D";
                        break;
                    case 0x3 :
                        inst0 = "LD B,E";
                        break;
                    case 0x4 :
                        inst0 = "LD B,H";
                        break;
                    case 0x5 :
                        inst0 = "LD B,L";
                        break;
                    case 0x6 :
                        inst0 = "LD B,(HL)";
                        break;
                    case 0x7 :
                        inst0 = "LD B,A";
                        break;
                    case 0x8 :
                        inst0 = "LD C,B";
                        break;
                    case 0x9 :
                        inst0 = "LD C,C";
                        break;
                    case 0xa :
                        inst0 = "LD C,D";
                        break;
                    case 0xb :
                        inst0 = "LD C,E";
                        break;
                    case 0xc :
                        inst0 = "LD C,H";
                        break;
                    case 0xd :
                        inst0 = "LD C,L";
                        break;
                    case 0xe :
                        inst0 = "LD C,(HL)";
                        break;
                    case 0xf :
                        inst0 = "LD C,A";
                        break;
                }
                break;
            case 0x5 :
                switch(nibble0) {
                    case 0x0 :
                        inst0 = "LD D,B";
                        break;
                    case 0x1 :
                        inst0 = "LD D,C";
                        break;
                    case 0x2 :
                        inst0 = "LD D,D";
                        break;
                    case 0x3 :
                        inst0 = "LD D,E";
                        break;
                    case 0x4 :
                        inst0 = "LD D,H";
                        break;
                    case 0x5 :
                        inst0 = "LD D,L";
                        break;
                    case 0x6 :
                        inst0 = "LD D,(HL)";
                        break;
                    case 0x7 :
                        inst0 = "LD D,A";
                        break;
                    case 0x8 :
                        inst0 = "LD E,B";
                        break;
                    case 0x9 :
                        inst0 = "LD E,C";
                        break;
                    case 0xa :
                        inst0 = "LD E,D";
                        break;
                    case 0xb :
                        inst0 = "LD E,E";
                        break;
                    case 0xc :
                        inst0 = "LD E,H";
                        break;
                    case 0xd :
                        inst0 = "LD E,L";
                        break;
                    case 0xe :
                        inst0 = "LD E,(HL)";
                        break;
                    case 0xf :
                        inst0 = "LD E,A";
                        break;
                }
                break;
            case 0x6 :
                switch(nibble0) {
                    case 0x0 :
                        inst0 = "LD H,B";
                        break;
                    case 0x1 :
                        inst0 = "LD H,C";
                        break;
                    case 0x2 :
                        inst0 = "LD H,D";
                        break;
                    case 0x3 :
                        inst0 = "LD H,E";
                        break;
                    case 0x4 :
                        inst0 = "LD H,H";
                        break;
                    case 0x5 :
                        inst0 = "LD H,L";
                        break;
                    case 0x6 :
                        inst0 = "LD H,(HL)";
                        break;
                    case 0x7 :
                        inst0 = "LD H,A";
                        break;
                    case 0x8 :
                        inst0 = "LD L,B";
                        break;
                    case 0x9 :
                        inst0 = "LD L,C";
                        break;
                    case 0xa :
                        inst0 = "LD L,D";
                        break;
                    case 0xb :
                        inst0 = "LD L,E";
                        break;
                    case 0xc :
                        inst0 = "LD L,H";
                        break;
                    case 0xd :
                        inst0 = "LD L,L";
                        break;
                    case 0xe :
                        inst0 = "LD L,(HL)";
                        break;
                    case 0xf :
                        inst0 = "LD L,A";
                        break;
                }
                break;
            case 0x7 :
                switch(nibble0) {
                    case 0x0 :
                        inst0 = "LD (HL),B";
                        break;
                    case 0x1 :
                        inst0 = "LD (HL),C";
                        break;
                    case 0x2 :
                        inst0 = "LD (HL),D";
                        break;
                    case 0x3 :
                        inst0 = "LD (HL),E";
                        break;
                    case 0x4 :
                        inst0 = "LD (HL),H";
                        break;
                    case 0x5 :
                        inst0 = "LD (HL),L";
                        break;
                    case 0x6 :
                        inst0 = "HALT";
                        break;
                    case 0x7 :
                        inst0 = "LD (HL),A";
                        break;
                    case 0x8 :
                        inst0 = "LD A,B";
                        break;
                    case 0x9 :
                        inst0 = "LD A,C";
                        break;
                    case 0xa :
                        inst0 = "LD A,D";
                        break;
                    case 0xb :
                        inst0 = "LD A,E";
                        break;
                    case 0xc :
                        inst0 = "LD A,H";
                        break;
                    case 0xd :
                        inst0 = "LD A,L";
                        break;
                    case 0xe :
                        inst0 = "LD A,(HL)";
                        break;
                    case 0xf :
                        inst0 = "LD A,A";
                        break;
                }
                break;
            case 0x8 :
                switch(nibble0) {
                    case 0x0 :
                        inst0 = "ADD A,B";
                        break;
                    case 0x1 :
                        inst0 = "ADD A,C";
                        break;
                    case 0x2 :
                        inst0 = "ADD A,D";
                        break;
                    case 0x3 :
                        inst0 = "ADD A,E";
                        break;
                    case 0x4 :
                        inst0 = "ADD A,H";
                        break;
                    case 0x5 :
                        inst0 = "ADD A,L";
                        break;
                    case 0x6 :
                        inst0 = "ADD A,(HL)";
                        break;
                    case 0x7 :
                        inst0 = "ADD A,A";
                        break;
                    case 0x8 :
                        inst0 = "ADC A,B";
                        break;
                    case 0x9 :
                        inst0 = "ADC A,C";
                        break;
                    case 0xa :
                        inst0 = "ADC A,D";
                        break;
                    case 0xb :
                        inst0 = "ADC A,E";
                        break;
                    case 0xc :
                        inst0 = "ADC A,H";
                        break;
                    case 0xd :
                        inst0 = "ADC A,L";
                        break;
                    case 0xe :
                        inst0 = "ADC A,(HL)";
                        break;
                    case 0xf :
                        inst0 = "ADC A,A";
                        break;
                }
                break;
            case 0x9 :
                switch(nibble0) {
                    case 0x0 :
                        inst0 = "SUB B";
                        break;
                    case 0x1 :
                        inst0 = "SUB C";
                        break;
                    case 0x2 :
                        inst0 = "SUB D";
                        break;
                    case 0x3 :
                        inst0 = "SUB E";
                        break;
                    case 0x4 :
                        inst0 = "SUB H";
                        break;
                    case 0x5 :
                        inst0 = "SUB L";
                        break;
                    case 0x6 :
                        inst0 = "SUB (HL)";
                        break;
                    case 0x7 :
                        inst0 = "SUB A";
                        break;
                    case 0x8 :
                        inst0 = "SBC A,B";
                        break;
                    case 0x9 :
                        inst0 = "SBC A,C";
                        break;
                    case 0xa :
                        inst0 = "SBC A,D";
                        break;
                    case 0xb :
                        inst0 = "SBC A,E";
                        break;
                    case 0xc :
                        inst0 = "SBC A,H";
                        break;
                    case 0xd :
                        inst0 = "SBC A,L";
                        break;
                    case 0xe :
                        inst0 = "SBC A,(HL)";
                        break;
                    case 0xf :
                        inst0 = "SBC A,A";
                        break;
                }
                break;
            case 0xa :
                switch(nibble0) {
                    case 0x0 :
                        inst0 = "AND B";
                        break;
                    case 0x1 :
                        inst0 = "AND C";
                        break;
                    case 0x2 :
                        inst0 = "AND D";
                        break;
                    case 0x3 :
                        inst0 = "AND E";
                        break;
                    case 0x4 :
                        inst0 = "AND H";
                        break;
                    case 0x5 :
                        inst0 = "AND L";
                        break;
                    case 0x6 :
                        inst0 = "AND (HL)";
                        break;
                    case 0x7 :
                        inst0 = "AND A";
                        break;
                    case 0x8 :
                        inst0 = "XOR B";
                        break;
                    case 0x9 :
                        inst0 = "XOR C";
                        break;
                    case 0xa :
                        inst0 = "XOR D";
                        break;
                    case 0xb :
                        inst0 = "XOR E";
                        break;
                    case 0xc :
                        inst0 = "XOR H";
                        break;
                    case 0xd :
                        inst0 = "XOR L";
                        break;
                    case 0xe :
                        inst0 = "XOR (HL)";
                        break;
                    case 0xf :
                        inst0 = "XOR A";
                        break;
                }
                break;
            case 0xb :
                switch(nibble0) {
                    case 0x0 :
                        inst0 = "OR B";
                        break;
                    case 0x1 :
                        inst0 = "OR C";
                        break;
                    case 0x2 :
                        inst0 = "OR D";
                        break;
                    case 0x3 :
                        inst0 = "OR E";
                        break;
                    case 0x4 :
                        inst0 = "OR H";
                        break;
                    case 0x5 :
                        inst0 = "OR L";
                        break;
                    case 0x6 :
                        inst0 = "OR (HL)";
                        break;
                    case 0x7 :
                        inst0 = "OR A";
                        break;
                    case 0x8 :
                        inst0 = "CP B";
                        break;
                    case 0x9 :
                        inst0 = "CP C";
                        break;
                    case 0xa :
                        inst0 = "CP D";
                        break;
                    case 0xb :
                        inst0 = "CP E";
                        break;
                    case 0xc :
                        inst0 = "CP H";
                        break;
                    case 0xd :
                        inst0 = "CP L";
                        break;
                    case 0xe :
                        inst0 = "CP (HL)";
                        break;
                    case 0xf :
                        inst0 = "CP A";
                        break;
                }
                break;
            case 0xc :
                switch(nibble0) {
                    case 0x0 :
                        inst0 = "RET NZ";
                        break;
                    case 0x1 :
                        inst0 = "POP BC";
                        break;
                    case 0x2 :
                        inst0 = "JP NZ,";
                        a16 = true;
                        break;
                    case 0x3 :
                        inst0 = "JP ";
                        a16 = true;
                        break;
                    case 0x4 :
                        inst0 = "CALL NZ,";
                        a16 = true;
                        break;
                    case 0x5 :
                        inst0 = "PUSH BC";
                        break;
                    case 0x6 :
                        inst0 = "ADD A,";
                        d8 = true;
                        break;
                    case 0x7 :
                        inst0 = "RST 00H";
                        break;
                    case 0x8 :
                        inst0 = "RET Z";
                        break;
                    case 0x9 :
                        inst0 = "RET";
                        break;
                    case 0xa :
                        inst0 = "JP Z,";
                        a16 = true;
                        break;
                    case 0xb :
                        cb = true;
                        break;
                    case 0xc :
                        inst0 = "CALL Z,";
                        a16 = true;
                        break;
                    case 0xd :
                        inst0 = "CALL ";
                        a16 = true;
                        break;
                    case 0xe :
                        inst0 = "ADC A,";
                        d8 = true;
                        break;
                    case 0xf :
                        inst0 = "RST 08H";
                        break;
                }
                break;
            case 0xd :
                switch(nibble0) {
                    case 0x0 :
                        inst0 = "RET NC";
                        break;
                    case 0x1 :
                        inst0 = "POP DE";
                        break;
                    case 0x2 :
                        inst0 = "JP NC,";
                        a16 = true;
                        break;
                    case 0x3 :
                        inst0 = "DATA?";
                        break;
                    case 0x4 :
                        inst0 = "CALL NC,";
                        a16 = true;
                        break;
                    case 0x5 :
                        inst0 = "PUSH DE";
                        break;
                    case 0x6 :
                        inst0 = "SUB ";
                        d8 = true;
                        break;
                    case 0x7 :
                        inst0 = "RST 10H";
                        break;
                    case 0x8 :
                        inst0 = "RET C";
                        break;
                    case 0x9 :
                        inst0 = "RETI";
                        break;
                    case 0xa :
                        inst0 = "JP C,";
                        a16 = true;
                        break;
                    case 0xb :
                        inst0 = "DATA?";
                        break;
                    case 0xc :
                        inst0 = "CALL C,";
                        a16 = true;
                        break;
                    case 0xd :
                        inst0 = "DATA?";
                        break;
                    case 0xe :
                        inst0 = "SBC A,";
                        d8 = true;
                        break;
                    case 0xf :
                        inst0 = "RST 18H";
                        break;
                }
                break;
            case 0xe :
                switch(nibble0) {
                    case 0x0 :
                        inst0 = "LDH (";
                        a8 = true;
                        inst1 = "),A";
                        break;
                    case 0x1 :
                        inst0 = "POP HL";
                        break;
                    case 0x2 :
                        inst0 = "LD (C),A";
                        break;
                    case 0x3 :
                        inst0 = "DATA?";
                        break;
                    case 0x4 :
                        inst0 = "DATA?";
                        break;
                    case 0x5 :
                        inst0 = "PUSH HL";
                        break;
                    case 0x6 :
                        inst0 = "AND ";
                        d8 = true;
                        break;
                    case 0x7 :
                        inst0 = "RST 20H";
                        break;
                    case 0x8 :
                        inst0 = "ADD SP,";
                        r8 = true;
                        break;
                    case 0x9 :
                        inst0 = "JP (HL)";
                        break;
                    case 0xa :
                        inst0 = "LD (";
                        a16 = true;
                        inst1 = "),A";
                        break;
                    case 0xb :
                        inst0 = "DATA?";
                        break;
                    case 0xc :
                        inst0 = "DATA?";
                        break;
                    case 0xd :
                        inst0 = "DATA?";
                        break;
                    case 0xe :
                        inst0 = "XOR ";
                        d8 = true;
                        break;
                    case 0xf :
                        inst0 = "RST 28H";
                        break;
                }
                break;
            case 0xf :
                switch(nibble0) {
                    case 0x0 :
                        inst0 = "LDH A,(";
                        a8 = true;
                        inst1 = ")";
                        break;
                    case 0x1 :
                        inst0 = "POP AF";
                        break;
                    case 0x2 :
                        inst0 = "LD A,(C)";
                        break;
                    case 0x3 :
                        inst0 = "DI";
                        break;
                    case 0x4 :
                        inst0 = "DATA?";
                        break;
                    case 0x5 :
                        inst0 = "PUSH AF";
                        break;
                    case 0x6 :
                        inst0 = "OR ";
                        d8 = true;
                        break;
                    case 0x7 :
                        inst0 = "RST 30H";
                        break;
                    case 0x8 :
                        inst0 = "LD HL,SP+";
                        r8 = true;
                        break;
                    case 0x9 :
                        inst0 = "LD SP,HL";
                        break;
                    case 0xa :
                        inst0 = "LD A,(";
                        a16 = true;
                        inst1 = ")";
                        break;
                    case 0xb :
                        inst0 = "EI";
                        break;
                    case 0xc :
                        inst0 = "DATA?";
                        break;
                    case 0xd :
                        inst0 = "DATA?";
                        break;
                    case 0xe :
                        inst0 = "CP ";
                        d8 = true;
                        break;
                    case 0xf :
                        inst0 = "RST 38H";
                        break;
                }
                break;
        }
        if (cb) {
            exit_eof(&c, gbfile, asmfile);
            byte = (char) c;
            nibble0 = c & 0b00001111;
            nibble1 = ((c & 0b11110000) >> 4) & (0b11110000);
            // CB case  
            switch(nibble1) {
            case 0x0 :
                switch(nibble0) {
                    case 0x0 :
                        break;
                    case 0x1 :
                        break;
                    case 0x2 :
                        break;
                    case 0x3 :
                        break;
                    case 0x4 :
                        break;
                    case 0x5 :
                        break;
                    case 0x6 :
                        break;
                    case 0x7 :
                        break;
                    case 0x8 :
                        break;
                    case 0x9 :
                        break;
                    case 0xa :
                        break;
                    case 0xb :
                        break;
                    case 0xc :
                        break;
                    case 0xd :
                        break;
                    case 0xe :
                        break;
                    case 0xf :
                        break;
                }
                break;
            case 0x1 :
                switch(nibble0) {
                    case 0x0 :
                        break;
                    case 0x1 :
                        break;
                    case 0x2 :
                        break;
                    case 0x3 :
                        break;
                    case 0x4 :
                        break;
                    case 0x5 :
                        break;
                    case 0x6 :
                        break;
                    case 0x7 :
                        break;
                    case 0x8 :
                        break;
                    case 0x9 :
                        break;
                    case 0xa :
                        break;
                    case 0xb :
                        break;
                    case 0xc :
                        break;
                    case 0xd :
                        break;
                    case 0xe :
                        break;
                    case 0xf :
                        break;
                }
                break;
            case 0x2 :
                switch(nibble0) {
                    case 0x0 :
                        break;
                    case 0x1 :
                        break;
                    case 0x2 :
                        break;
                    case 0x3 :
                        break;
                    case 0x4 :
                        break;
                    case 0x5 :
                        break;
                    case 0x6 :
                        break;
                    case 0x7 :
                        break;
                    case 0x8 :
                        break;
                    case 0x9 :
                        break;
                    case 0xa :
                        break;
                    case 0xb :
                        break;
                    case 0xc :
                        break;
                    case 0xd :
                        break;
                    case 0xe :
                        break;
                    case 0xf :
                        break;
                }
                break;
            case 0x3 :
                switch(nibble0) {
                    case 0x0 :
                        break;
                    case 0x1 :
                        break;
                    case 0x2 :
                        break;
                    case 0x3 :
                        break;
                    case 0x4 :
                        break;
                    case 0x5 :
                        break;
                    case 0x6 :
                        break;
                    case 0x7 :
                        break;
                    case 0x8 :
                        break;
                    case 0x9 :
                        break;
                    case 0xa :
                        break;
                    case 0xb :
                        break;
                    case 0xc :
                        break;
                    case 0xd :
                        break;
                    case 0xe :
                        break;
                    case 0xf :
                        break;
                }
                break;
            case 0x4 :
                switch(nibble0) {
                    case 0x0 :
                        break;
                    case 0x1 :
                        break;
                    case 0x2 :
                        break;
                    case 0x3 :
                        break;
                    case 0x4 :
                        break;
                    case 0x5 :
                        break;
                    case 0x6 :
                        break;
                    case 0x7 :
                        break;
                    case 0x8 :
                        break;
                    case 0x9 :
                        break;
                    case 0xa :
                        break;
                    case 0xb :
                        break;
                    case 0xc :
                        break;
                    case 0xd :
                        break;
                    case 0xe :
                        break;
                    case 0xf :
                        break;
                }
                break;
            case 0x5 :
                switch(nibble0) {
                    case 0x0 :
                        break;
                    case 0x1 :
                        break;
                    case 0x2 :
                        break;
                    case 0x3 :
                        break;
                    case 0x4 :
                        break;
                    case 0x5 :
                        break;
                    case 0x6 :
                        break;
                    case 0x7 :
                        break;
                    case 0x8 :
                        break;
                    case 0x9 :
                        break;
                    case 0xa :
                        break;
                    case 0xb :
                        break;
                    case 0xc :
                        break;
                    case 0xd :
                        break;
                    case 0xe :
                        break;
                    case 0xf :
                        break;
                }
                break;
            case 0x6 :
                switch(nibble0) {
                    case 0x0 :
                        break;
                    case 0x1 :
                        break;
                    case 0x2 :
                        break;
                    case 0x3 :
                        break;
                    case 0x4 :
                        break;
                    case 0x5 :
                        break;
                    case 0x6 :
                        break;
                    case 0x7 :
                        break;
                    case 0x8 :
                        break;
                    case 0x9 :
                        break;
                    case 0xa :
                        break;
                    case 0xb :
                        break;
                    case 0xc :
                        break;
                    case 0xd :
                        break;
                    case 0xe :
                        break;
                    case 0xf :
                        break;
                }
                break;
            case 0x7 :
                switch(nibble0) {
                    case 0x0 :
                        break;
                    case 0x1 :
                        break;
                    case 0x2 :
                        break;
                    case 0x3 :
                        break;
                    case 0x4 :
                        break;
                    case 0x5 :
                        break;
                    case 0x6 :
                        break;
                    case 0x7 :
                        break;
                    case 0x8 :
                        break;
                    case 0x9 :
                        break;
                    case 0xa :
                        break;
                    case 0xb :
                        break;
                    case 0xc :
                        break;
                    case 0xd :
                        break;
                    case 0xe :
                        break;
                    case 0xf :
                        break;
                }
                break;
            case 0x8 :
                switch(nibble0) {
                    case 0x0 :
                        break;
                    case 0x1 :
                        break;
                    case 0x2 :
                        break;
                    case 0x3 :
                        break;
                    case 0x4 :
                        break;
                    case 0x5 :
                        break;
                    case 0x6 :
                        break;
                    case 0x7 :
                        break;
                    case 0x8 :
                        break;
                    case 0x9 :
                        break;
                    case 0xa :
                        break;
                    case 0xb :
                        break;
                    case 0xc :
                        break;
                    case 0xd :
                        break;
                    case 0xe :
                        break;
                    case 0xf :
                        break;
                }
                break;
            case 0x9 :
                switch(nibble0) {
                    case 0x0 :
                        break;
                    case 0x1 :
                        break;
                    case 0x2 :
                        break;
                    case 0x3 :
                        break;
                    case 0x4 :
                        break;
                    case 0x5 :
                        break;
                    case 0x6 :
                        break;
                    case 0x7 :
                        break;
                    case 0x8 :
                        break;
                    case 0x9 :
                        break;
                    case 0xa :
                        break;
                    case 0xb :
                        break;
                    case 0xc :
                        break;
                    case 0xd :
                        break;
                    case 0xe :
                        break;
                    case 0xf :
                        break;
                }
                break;
            case 0xa :
                switch(nibble0) {
                    case 0x0 :
                        break;
                    case 0x1 :
                        break;
                    case 0x2 :
                        break;
                    case 0x3 :
                        break;
                    case 0x4 :
                        break;
                    case 0x5 :
                        break;
                    case 0x6 :
                        break;
                    case 0x7 :
                        break;
                    case 0x8 :
                        break;
                    case 0x9 :
                        break;
                    case 0xa :
                        break;
                    case 0xb :
                        break;
                    case 0xc :
                        break;
                    case 0xd :
                        break;
                    case 0xe :
                        break;
                    case 0xf :
                        break;
                }
                break;
            case 0xb :
                switch(nibble0) {
                    case 0x0 :
                        break;
                    case 0x1 :
                        break;
                    case 0x2 :
                        break;
                    case 0x3 :
                        break;
                    case 0x4 :
                        break;
                    case 0x5 :
                        break;
                    case 0x6 :
                        break;
                    case 0x7 :
                        break;
                    case 0x8 :
                        break;
                    case 0x9 :
                        break;
                    case 0xa :
                        break;
                    case 0xb :
                        break;
                    case 0xc :
                        break;
                    case 0xd :
                        break;
                    case 0xe :
                        break;
                    case 0xf :
                        break;
                }
                break;
            case 0xc :
                switch(nibble0) {
                    case 0x0 :
                        break;
                    case 0x1 :
                        break;
                    case 0x2 :
                        break;
                    case 0x3 :
                        break;
                    case 0x4 :
                        break;
                    case 0x5 :
                        break;
                    case 0x6 :
                        break;
                    case 0x7 :
                        break;
                    case 0x8 :
                        break;
                    case 0x9 :
                        break;
                    case 0xa :
                        break;
                    case 0xb :
                        break;
                    case 0xc :
                        break;
                    case 0xd :
                        break;
                    case 0xe :
                        break;
                    case 0xf :
                        break;
                }
                break;
            case 0xd :
                switch(nibble0) {
                    case 0x0 :
                        break;
                    case 0x1 :
                        break;
                    case 0x2 :
                        break;
                    case 0x3 :
                        break;
                    case 0x4 :
                        break;
                    case 0x5 :
                        break;
                    case 0x6 :
                        break;
                    case 0x7 :
                        break;
                    case 0x8 :
                        break;
                    case 0x9 :
                        break;
                    case 0xa :
                        break;
                    case 0xb :
                        break;
                    case 0xc :
                        break;
                    case 0xd :
                        break;
                    case 0xe :
                        break;
                    case 0xf :
                        break;
                }
                break;
            case 0xe :
                switch(nibble0) {
                    case 0x0 :
                        break;
                    case 0x1 :
                        break;
                    case 0x2 :
                        break;
                    case 0x3 :
                        break;
                    case 0x4 :
                        break;
                    case 0x5 :
                        break;
                    case 0x6 :
                        break;
                    case 0x7 :
                        break;
                    case 0x8 :
                        break;
                    case 0x9 :
                        break;
                    case 0xa :
                        break;
                    case 0xb :
                        break;
                    case 0xc :
                        break;
                    case 0xd :
                        break;
                    case 0xe :
                        break;
                    case 0xf :
                        break;
                }
                break;
            case 0xf :
                switch(nibble0) {
                    case 0x0 :
                        break;
                    case 0x1 :
                        break;
                    case 0x2 :
                        break;
                    case 0x3 :
                        break;
                    case 0x4 :
                        break;
                    case 0x5 :
                        break;
                    case 0x6 :
                        break;
                    case 0x7 :
                        break;
                    case 0x8 :
                        break;
                    case 0x9 :
                        break;
                    case 0xa :
                        break;
                    case 0xb :
                        break;
                    case 0xc :
                        break;
                    case 0xd :
                        break;
                    case 0xe :
                        break;
                    case 0xf :
                        break;
                }
                break;
            }
        }

        //Process Immediates
        if (d8 || a8 || r8 || d16 || a16) {
            if (d8 || a8)
            {    
                exit_eof(&c, gbfile, asmfile); 
                unsigned char immediate_unsigned = (unsigned char) c;        
                fprintf(asmfile,"0x%x%x : %s0x%x%s\n", nibble1, nibble0, inst0, immediate_unsigned, inst1 == NULL ? "" : inst1);
            } else if ( r8 ) {
                exit_eof(&c, gbfile, asmfile); 
                signed char immediate_signed = (signed char) c;
                if (offset) {
                    fprintf(asmfile,"0x%x%x : %s%d + 0xFF00%s\n", nibble1, nibble0, inst0, immediate_signed, inst1 == NULL ? "" : inst1);
                } else {    
                    fprintf(asmfile,"0x%x%x : %s%d%s\n", nibble1, nibble0, inst0, immediate_signed, inst1 == NULL ? "" : inst1);
                }
            } else if (d16 || a16) {
                exit_eof(&c, gbfile, asmfile);
                unsigned short immediate_unsigned = (unsigned short) c;        
                exit_eof(&c, gbfile, asmfile); 
                immediate_unsigned = (immediate_unsigned << 8) | ((unsigned short) c);
                fprintf(asmfile,"0x%x%x : %s0x%x%s\n", nibble1, nibble0, inst0, immediate_unsigned, inst1 == NULL ? "" : inst1);
            } else {
                perror("default case shouldn't happen");
                fclose(gbfile);
                fclose(asmfile);
                return -1;
            }
        } else {
            if (cb) {
                fprintf(asmfile, "%s : 0x%x%x : %s \n", "0xCB", nibble1, nibble0, inst0);
            } else {
                fprintf(asmfile, "0x%x%x : %s \n", nibble1, nibble0, inst0);
            }
        }
        c = getc(gbfile);
    }
    fclose(gbfile);
    fclose(asmfile);
    return 0;
}

void exit_eof(int * c, FILE* gbfile, FILE* asmfile)
{
    *c = getc(gbfile);
    if (*c == EOF)
    {
        perror("EOF");
        fclose(gbfile);
        fclose(asmfile);
        exit(-1);
    }
}

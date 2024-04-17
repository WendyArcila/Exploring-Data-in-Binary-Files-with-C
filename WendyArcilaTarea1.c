#include<stdio.h>
#include<stdint.h>

struct student 
{
    unsigned int studentId;
    unsigned char flag; 
    char name[sizeof(char)*23]; 
    unsigned int age; 
};
enum FG {
    FEMALE = 1 << 7,
    POSTGRAD = 1 << 6 
};


void printStudent(struct student student){
    enum FG female = FEMALE;   
    if(student.flag & female){
        printf(" %-30s  %-15d                femenino", student.name, student.age);
    }else {
        printf(" %-30s  %-15d               masculino", student.name, student.age); 
    }
    putchar('\n');
    putchar('\n');
}

int numberOfStudents(int pos, FILE* file ){
    fseek(file, pos , SEEK_SET);
    int totalStudent; 
    fread(&totalStudent, sizeof(int), 1, file); 
    printf("total de estudiantes:  %d \n", totalStudent); 
    return totalStudent; 
}

void saveStudent(struct student* allStudents, int totalStudent, FILE* file){
    int i, total;
    for(i = 0; i < totalStudent; i++){
        int total = fread(&allStudents[i], sizeof(struct student), 1, file); 
        if (total != 1){
        printf("Error de lectura"); 
        };
    }  
}

void filter(struct student* allStudents, int totalStudent, int min, int max){
    int i; 
    printf("   Name                       Age                        Gender\n "); 
    for(i = 0; i < totalStudent; i++){
        if(allStudents[i].age >= min && allStudents[i].age <= max ){
            printStudent( allStudents[i]);
        }
    }
}

int main(){
    //Se solicita el nombre del archivo y los valores 
    char fileName[30]; 
    int min, max; 
    printf("Por favor ingrese el nombre del archivo a evaluar, incluya la extension (.bin), luego incluya el numero minimo del filtro y por ultimo el maximo \n");
    scanf("%s %d %d", &fileName, &min, &max);
    printf("fileName: %s \n", fileName);

    //Accede al archivo .bin 
    FILE *file = fopen(fileName, "rb");
    if(file == NULL){
        printf("Error inicial de lectura ");
    }
    
    //Conocemos la cantidad de estudiantes
    int totalStudent = numberOfStudents( 2, file ); 

    //Se lleva el puntero al primer estudiante 
    fseek(file, 14 , SEEK_SET);

    //Se crea el array de todos los estudiantes
    struct student allStudents[totalStudent];   

    //Se almacenan todos los estudiantes
    saveStudent(allStudents, totalStudent, file); 

    //Se filtran los estudiantes
    filter(allStudents, totalStudent, min, max);

    fclose(file); 

    return 0;
}
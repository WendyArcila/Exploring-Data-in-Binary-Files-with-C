#include<stdio.h>
#include<stdint.h>
#include <stdlib.h> 

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
struct course {
    unsigned int courseId;
    char name [sizeof(char)*32];
    int hours; 
}; 

struct enrollment {
    unsigned int idStudent;
    unsigned int idCourse; 
    int year;
    int semester; 
};

int numberOfStudents(int pos, FILE* file ){
    fseek(file, pos , SEEK_SET);
    int totalStudent; 
    fread(&totalStudent, sizeof(int), 1, file); 
    printf("total de estudiantes:  %d \n", totalStudent); 
    return totalStudent; 
}

int numberOfCourses(FILE* file){
    int totalCourses;
    fread(&totalCourses, sizeof(int), 1, file);
    printf("total de cursos:  %d \n", totalCourses); 
    return totalCourses; 
}

int numberOfEnrollment(FILE* file){
    int totalEnrollment; 
    fread(&totalEnrollment, sizeof(int), 1, file); 
    printf("total de matriculas:  %d \n", totalEnrollment);
    return totalEnrollment; 
}

void saveStudent(struct student* allStudents, int totalStudent, FILE* file){
    int i, total;
    printf("Entra a guardar estudiantes\n");
    for(i = 0; i < totalStudent; i++){
        int total = fread(&allStudents[i], sizeof(struct student), 1, file); 
        if (total != 1){
        printf("Error de lectura"); 
        break;
        };
    }  
}

void saveCourses(struct course* allCourses, int totalCourses, FILE* file){
    int i, total;
    printf("Entra a guardar cursos\n");
    for(i = 0; i < totalCourses; i++){
        int total = fread(&allCourses[i], sizeof(struct course), 1, file); 
        if (total != 1){
            printf("Error de lectura");
            break; 
        };
    }  
}

void saveEnrollment(struct enrollment* allEnrollment, int totalEnrollment, FILE* file){
    int i, total;
    printf("Entra a guardar matriculas\n");
    for(i = 0; i < totalEnrollment; i++){
        int total = fread(&allEnrollment[i], sizeof(struct enrollment), 1, file); 
        if (total != 1){
            printf("Error de lectura matriculas"); 
            break; 
        }; 
    }  
}

void printData(year, semester, maleUnderg, femaleUnderg, maleG, femaleG){
    printf("\n year: %d semester: %d  male underg: %d  female underg: %d  maleG: %d female: %d\n", year, semester, maleUnderg, femaleUnderg, maleG, femaleG); 
    putchar('\n');
    putchar('\n');
}

void isSameStudent( char flag, int* total){
      
     enum FG female = FEMALE; 
     enum FG grade = POSTGRAD;
    
    if( (female & flag) && (flag & grade)){
        total[3] += 1;   
    }else if (female & flag){
        total[1] += 1;    
    }else if (grade & flag){
        total[2] += 1;  
    }else{
        total[0] += 1;  
    }

}

void semesterFilter(struct enrollment* allEnrollment, int totalEnrollment, int year, struct student* student, int totalStudents, int* total ){
    int i, j, k;   
    int totalIdStudent[totalStudents]; 
    int *ptrTotalIdStudent = totalIdStudent;
    for(k = 0; k < 4; k++){
        for(i = 0; i < totalEnrollment; i++){
        if(year == allEnrollment[i].year){
            if(allEnrollment[i].semester == k+1){ 
               for(j = 0; j < totalStudents; j++){
                    if(allEnrollment[i].idStudent == student[j].studentId){ 
                        if(allEnrollment[i-1].idStudent != allEnrollment[i].idStudent){ 
                            isSameStudent(student[j].flag, total); 
                        }
                    }
               }
            }
        }
    }
    printData(year, k+1, total[0], total[1], total[2], total[3] ); 
    }
    total[0]= 0;
    total[1]= 0; 
    total[2]= 0;
    total[3]= 0; 
 }
    



int main(){
    //Se solicita el nombre del archivo 
    char fileName[30]; 
    printf("Por favor ingrese el nombre del archivo a evaluar, incluya la extension (.bin) \n");
    scanf("%s", &fileName);
    printf("fileName: %s \n", fileName);

    //Accede al archivo .bin 
    FILE *file = fopen(fileName, "rb");
    if(file == NULL){
        printf("Error inicial de lectura ");
    }
    
    //Conocemos la cantidad de estudiantes, cursos y matriculas
    int totalStudent = numberOfStudents( 2, file ); 
    int totalCourses = numberOfCourses (file);
    int totalEnrollment = numberOfEnrollment(file); 

    //Se lleva el puntero al primer estudiante 
    fseek(file, 14 , SEEK_SET);

    //Se crea el array y punteros de estudiantes, cursos y matriculas
    struct student allStudents[totalStudent];  
    struct student *ptrStudent = allStudents; 
    struct course allCourses[totalCourses]; 
    struct course *ptrCourse = allCourses; 

    //Se crea espacio en memoria para las matriculas 
    struct enrollment *ptrEnrollment;
    ptrEnrollment = (struct enrollment *) malloc (totalEnrollment * sizeof(struct enrollment));
    if  (ptrEnrollment == NULL){
        printf("Error al asignar memoria"); 
    }

    //Se almacenan todos los datos
    saveStudent(allStudents, totalStudent, file); 
    saveCourses(allCourses, totalCourses, file); 
    saveEnrollment(ptrEnrollment, totalEnrollment, file);

    int i; 
    int year = 2020;
    int total[4] = {0, 0, 0, 0}; 
    int *ptrTotal = total; 

    for(i = 0; i < 10; i++){
         semesterFilter(ptrEnrollment, totalEnrollment, year, ptrStudent, totalStudent, ptrTotal);
         year++; 
    }
    

    free(ptrStudent); 
    free(ptrCourse); 
    free(ptrEnrollment); 

    fclose(file); 

    return 0;
}
#include<stdio.h>
#include<stdint.h>
#include <stdlib.h> 
#include <string.h>


struct student {
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
    //struct student idStudent; 
    //struct course idCourse; 
    int year;
    int semester; 
};

void printStudent(struct student student){
    printf("Nombre: %s, edad: %d", student.name, student.age);
    putchar('\n');
    putchar('\n');
}

void printCourses(struct course course){
    printf("Nombre: %s, horas: %d", course.name, course.hours);
    putchar('\n');
    putchar('\n');
}

void printEnrollment(struct enrollment enrollment){
    printf("Estudiante: %d, Curso: %d , anio: %d, semestre: %d", enrollment.idStudent, enrollment.idCourse,  enrollment.year, enrollment.semester);
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
        //printEnrollment(allEnrollment[i]); 
 
    }  
}

/*void filterCourses(struct enrollment* allEnrollment, int totalEnrollment, struct enrollment* filteredCourse, int num){
    printf("entra al filtro");
    int i, j = 0; 
    for(i = 0; i < totalEnrollment; i++){
        if(num + 1 == allEnrollment[i].idCourse){
            printEnrollment(allEnrollment[i]); 
            filteredCourse[j].idStudent =  allEnrollment[i].idStudent;  
            filteredCourse[j].idCourse =  allEnrollment[i].idCourse; 
            filteredCourse[j].year =  allEnrollment[i].year; 
            filteredCourse[j].semester =  allEnrollment[i].semester; 
            j += 1; 
           printEnrollment(filteredCourse[j]); 
        }
    }
}
*/
void createSpace(struct enrollment** filterC, int size){
    int i; 
    for(i = 0; i < size; i++){
        filterC[i] = (struct enrollment*) malloc (50 * sizeof(struct enrollment));
        if(filterC[i] == NULL){
            printf("Error al asignar memoria"); 
        }
    }   
}
float average(int num, struct enrollment* allEnrollment, struct student* student, int totalEnrollment, int totalStudents){
   int i, j, totalSum=0, totalStud = 0; 
   for (i = 0; i < totalEnrollment; i++) {
    if(num == allEnrollment[i].idCourse){
        for (j = 0; j < totalStudents; j++){
            if(allEnrollment[i].idStudent == student[j].studentId){
                totalSum += student[j].age; 
                totalStud ++;
                //printf("suma: %d y estudiantes: %d", totalSum, totalStud); 
            }
        }
    }
   }

   return (float)totalSum/(float)totalStud; 
}



void printInfo(char* name, float average){
    printf("%-35s   %-10f", name, average); 
    putchar('\n');
    putchar('\n');
}


int main(){
    //Se solicita el nombre del archivo 
    char fileName[30]; 
    
    printf("Por favor ingrese el nombre del archivo a evaluar, incluya la extension (.bin)\n");
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

    //Se crea el array de estudiantes, cursos y matriculas
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
    char name[32]; 

    printf("Course name:                          Average: \n"); 
    for(i = 0; i < totalCourses; i++){
        float averageAge = average(i+1, ptrEnrollment, ptrStudent, totalEnrollment, totalStudent);
        strcpy(name,  ptrCourse[i].name); 
        printInfo( name, averageAge); 
    }
    

    //Se crea el espacio en memoria para filtrar los cursos 
    //struct enrollment *filteredCourse = (struct enrollment*) malloc (totalCourses * sizeof(struct enrollment));
    //createSpace(filteredCourse, totalCourses); 
    //filterCourses( ptrEnrollment, totalEnrollment, filteredCourse, 0);

    /*int i; 
    for(i = 0; i < totalCourses; i++){
        free(filteredCourse[i]);
    }*/

    free(ptrStudent); 
    free(ptrCourse); 
    free(ptrEnrollment); 

    fclose(file); 

    return 0;
}
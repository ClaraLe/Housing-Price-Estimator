#include<stdio.h>
#include<stdlib.h>

/*program must learn the weights of each category after inputting training data
W = (X^T * X)^-1 * X^T * Y
*/


int main(int argc, char** argv){
  FILE* fp = NULL;
  char* filename = NULL;
  int noTrain, columns, i, r, c, test, noAtt;
  double sum, e;
  filename = argv[1];
  fp = fopen(filename, "r");
  if(fp == NULL){
    printf("error\n");
    return 0;
  }
  fscanf(fp, "%d\n", &noAtt);
  fscanf(fp, "%d\n", &noTrain);

  columns = noAtt+1;
  int wcolumns = columns*2;
  

  double** train = (double**) malloc(noTrain*sizeof(double*)); //X
  for (i = 0; i < noTrain; i++){
    train[i] = (double*) malloc(columns*sizeof(double));
  }

  double** trainT = (double**) malloc(columns*sizeof(double*)); //X^T
  for(i = 0; i < columns; i++){
    trainT[i] = (double*) malloc(noTrain*sizeof(double));
  }

  double** trainPrice = (double**) malloc(noTrain*sizeof(double*)); //Y
  for(i = 0; i < noTrain; i++){
    trainPrice[i] = (double*) malloc(sizeof(double*));
  }

  double** weight = (double**) malloc(noTrain*sizeof(double*)); //W
  for(i = 0; i < noTrain; i++){
    weight[i] = (double*) malloc(sizeof(double));
  }

  double** result = (double**) malloc(columns*sizeof(double*)); //X^T*X
  for(i = 0; i < columns; i++){
    result[i] = (double*) malloc(wcolumns*sizeof(double));
  }

  double** inverse = (double**) malloc(columns*sizeof(double*)); //(X^T*X)^-1
  for(i = 0; i < columns; i++){
    inverse[i] = (double*) malloc(columns*sizeof(double));
  }

  double** product = (double**) malloc(columns*sizeof(double*));
  for(i = 0; i < columns; i++){
    product[i] = (double*) malloc(noTrain*sizeof(double));
  }

  for(r = 0; r < noTrain; r++){
    train[r][0] = 1;
    trainT[0][r] = 1;
  }
  e = 0;
  for(r = 0; r < noTrain; r++){
    for(c = 1; c < columns+1; c++){
      if (c == columns){
	fscanf(fp, "%lf\n", &e);
	trainPrice[r][0] = e;
      }
      else{
	fscanf(fp, "%lf%*[,]", &e);
	train[r][c] = e;
	trainT[c][r] = e;
	}
    }
  }

 

  //trainPrice = Y  train = X trainT = X^T
  // y= w0 + w1*x1+w2*x2+w3*x3+w4*x4 W = (X^T*X)^1 * X^T * Y
  sum = 0.0;
  for(r = 0; r < columns; r++){
    for(c = 0; c < columns; c++){
      for(i = 0; i < noTrain; i++){
	sum += trainT[r][i]*train[i][c]; //check the variables
      }
      result[r][c] = sum;
      sum = 0.0;
    }
  }
  
  //find inverse of result
  //augment matrix -> turn left side of augmented matrix to identity matrix
  for(r = 0; r < columns; r++){
    for(c = columns; c < columns*2; c++){
      if(c == columns+r){
	result[r][c] = 1;
      }
      else{
	result[r][c] = 0;
      }
    }
  }

    //want to make pivot 1 and everything else in that column 0
    i = 0;
    int c1;
    int r1;
    double z;
    double one = 1.0;
    //gauss jordan elimination
    //make pivot in column than make every other number in that column zero
    for(r = 0; r< columns; r++){
      for(c = 0; c <columns; c++){
	if(r == c){
	  if(result[r][c] != one){
	    z = result[r][c];
	    for(c1 = 0; c1 < columns*2; c1++){
	      result[r][c1] = result[r][c1]/z;
	    }
	  }
	  for(r1 = 0; r1 < columns; r1++){
	    if (r1 != r && result[r1][c] != 0){
	      z = result[r1][c];
	      for(c1 = 0; c1 < wcolumns; c1++){
		result[r1][c1] = result[r1][c1] - z*result[r][c1];
	      }
	    }
	  }
	}
      }
    } 
    i = 0;

    for(r = 0; r < columns; r++){
      for(c = columns; c < wcolumns; c++){
	  inverse[r][i] = result[r][c];
	  i += 1;
      }
      i = 0;
    }

 
    sum = 0.0;
    //inverse * X^T * Y = W
    for(r = 0; r < columns; r++){
      for(c = 0; c < noTrain; c++){
	for(i = 0; i < columns; i++){
	  sum += inverse[r][i]*trainT[i][c];
	}
	product[r][c] = sum;
	sum = 0.0;
      }
    }
   
    sum = 0.0;
    for(r = 0; r < columns; r++){
      for(c = 0; c < noTrain; c++){
	for(i = 0; i < noTrain; i++){
	  sum += product[r][i]*trainPrice[i][0];
	}
	weight[r][0] = sum;
	sum = 0.0;
      }
    }
   
    filename = argv[2];
    fp = fopen(filename, "r");

    if(fp == NULL){
      printf("error\n");
      return 0;
    }
    fscanf(fp, "%d\n", &test);

    double** testing = (double**) malloc (test*sizeof(double*));
    for(i = 0; i < test; i++){
      testing[i] = (double*) malloc(columns*sizeof(double));
    }
    for(r = 0; r < test; r++){
      testing[r][0] = 1.0;
    }

    for(r = 0; r < test; r++){
      for(c = 1; c < columns; c++){
	if(c == columns - 1){
	  fscanf(fp, "%lf\n", &e);
	  testing[r][c] = e;
	}
	else{
	  fscanf(fp, "%lf%*[,]", &e);
	  testing[r][c] = e;
	}
      }
    }

    sum = 0.0;
    for (r = 0; r < test; r++){
      for(c = 0; c < columns; c++){
       	sum += testing[r][c]*weight[c][0];
	}
      printf("%0.0lf\n", sum);
      sum = 0.0;
    }


    free(train);
    free(trainT);
    free(weight);
    free(testing);
    free(trainPrice);
    free(inverse);
    free(product);
  return 0;
}

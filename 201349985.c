#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int findIndex(char val){
	if (val == ' ') return 26;
	char arr[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
	int mid, ind = -1, low = 0, max = 25;
	while(low <= max){
		mid = low + (max-low)/2;
		if (arr[mid] == val){
			ind=mid;
			break;
		}else{
			if(arr[mid] < val) low = mid+1;
			else max = mid-1;
		}
	}return ind;
}

struct Number{
	char sign;
	char data[400];
	int tail;
	char remainder[400];
	int r_tail;
};	

int getZeroes(char *A, int A_tail){
	int counter = 0;
	
	if (A[0] != '0') return A_tail;
	if (A_tail == 0) return A_tail;
	for (int i = 0; i <= A_tail ; ++i){
		if(A[i] == '0') counter++;
		else i = A_tail+1;
	}

	for (int j = 0; j <= A_tail-counter; ++j) A[j] = A[counter+j];
	
	if(A_tail-counter < 0){
		A[0] = '0';
		A[1] = '\0';
		return 0;
	}
	
	A[A_tail-counter+1] = '\0';
	
	return A_tail-counter;
}

void reverse(char *A, int a_tail){
	int a = 0, b = a_tail;
	char temp;
	while(a < b){
		temp = A[a];
		A[a] = A[b];
		A[b] = temp;
		a++;
		b--;
	}
}

struct Number Multiply(struct Number A, struct Number  B){
	struct Number result1;
	result1.tail = 0;
	int p_value = 0, j=0, prod, carry = 0; //i-append na agad sa start
	
	for (int i = 0; i < 400; ++i) result1.data[i] = '0';
	while(B.tail >= 0){ //starts sa dulo (not reverse yung input)
		carry = 0;
		for (int i = A.tail; i >= 0; --i){
			prod = (B.data[B.tail] - '0') * (A.data[i] - '0') + (carry + (result1.data[j] - '0') );
			carry = prod/10;
			prod = prod%10;
			result1.data[j] = prod + '0'; //output is reverse
			j++;
		}
		while (carry > 0){
			prod = carry%10;
			carry = carry/10;
			result1.data[j] = prod + '0';
			j++;
		}

		if (j > result1.tail) result1.tail = j-1;
		B.tail--;
		p_value++;
		j = p_value;

	} 
	while (carry > 0){
		prod = carry%10;
		carry = carry/10;
		result1.tail++;
		result1.data[result1.tail] = prod + '0';
	}

	result1.data[result1.tail+1] = '\0';
	reverse(result1.data, result1.tail);
	result1.tail = getZeroes(result1.data, result1.tail);
	return result1;

	 
}

struct Number Add(struct Number A, struct Number B){
	int i = 0, sum = 0, carry = 0;
	struct Number result1;
	result1.sign = '+';
	while(A.tail >= 0 && B.tail >= 0){ //takes in NOT reverse string
		sum = (A.data[A.tail] - '0') + (B.data[B.tail] - '0') + carry;
		carry = sum/10;
		sum = sum%10;
		result1.data[i] = sum + '0';
		i++;
		A.tail--;
		B.tail--;		
	}while(A.tail >= 0){
		// sum = (result[i] - '0') + carry;
		sum = (A.data[A.tail] - '0') + carry;
		carry = sum/10;
		sum = sum%10;
		result1.data[i] = sum + '0';
		i++;
		A.tail--;
	}
	if (carry > 0){
		result1.data[i] = carry + '0';
		i++;
	}
	result1.data[i] = '\0';
	reverse(result1.data, i-1);
	result1.tail = getZeroes(result1.data, i-1);
	return result1;
}

struct Number Subtract(struct Number A, struct Number B){
	struct Number result1;
	
	int borrow = 0, diff, i = 0, j;
	while(A.tail >= 0 && B.tail >= 0){
		if (borrow != 0){
			j = A.tail;
			while(A.data[j] == '0'){
				A.data[j] = '9';
				j--;
			}
			A.data[j] = ((A.data[j] - '0') -1) + '0';
			borrow = 0;
		}
		if (A.data[A.tail] < B.data[B.tail]) borrow = 10;
		diff = (A.data[A.tail] - '0') - (B.data[B.tail] - '0') + borrow;
		result1.data[i] = diff + '0';
		A.tail--;
		B.tail--;
		i++;
	}
	while(A.tail >= 0){
		if (borrow != 0){ 
			j = A.tail;
			while(A.data[j] == '0'){
				//lahat ng zero gawing 9
				A.data[j] = '9';
				j--;
			}
			A.data[j] = ((A.data[j] - '0') -1) + '0';
			borrow = 0;
		}result1.data[i] = A.data[A.tail];
		i++;
		A.tail--;
	}result1.data[i] = '\0';
	reverse(result1.data, i-1);
	result1.tail = getZeroes(result1.data, i-1);
	return result1;
}

int Greater(struct Number A, struct Number B){
	if (A.tail > B.tail) return 1;
	else if (B.tail > A.tail) return -1;
	for (int i = 0; i <= A.tail; ++i){
		if (A.data[i] > B.data[i]) return 1;
		else if (A.data[i] < B.data[i]) return -1;
	}return 1; //equal sila
}


struct Number Divide(struct Number A, struct Number B){
	int counter, i, a;
	struct Number temp, result;
	// result.sign = '+', temp.sign = '+';
	a = Greater(A, B);
	if(a != 1){
		result.r_tail = A.tail;
		strcpy(result.remainder, A.data);
		strcpy(result.data, "0\0");
		result.tail = 0;
		return result;
	}
	struct Number prod, div, quotient;
	div.tail = B.tail;

	// div = B; 
	for (i = 0; i <= B.tail; ++i) div.data[i] = A.data[i];
	a = Greater(div, B);
	if(a != 1){
		div.data[i] = A.data[i];
		i++, div.tail++;
	}div.data[i] = '\0';
	strcpy(prod.data, "1\0");
	prod.tail = 0, quotient.tail = 0;
	while(1){
		counter = 0;
		while(1){
			a = Greater(div, B);
			if (a != 1) break;
			div = Subtract(div, B);
			counter++;
		}
		quotient.data[quotient.tail] = counter + '0';
		quotient.data[quotient.tail+1] = '\0';

		quotient.tail++, div.tail++;
		div.data[div.tail] = A.data[i];
		div.tail = getZeroes(div.data, div.tail);
		div.data[div.tail+1] = '\0';
		if(i > A.tail) break;
		i++;
	}
	//remainder yung nasa div
	//quotient yung nasa quotient
	quotient.data[quotient.tail] = '\0';
	strcpy(result.data, quotient.data);
	result.tail = quotient.tail-1;
	if (div.data[0] == '\0'){
		 strcpy(div.data, "0\0");
		 div.tail=1;
	}
	strcpy(result.remainder, div.data);
	result.r_tail = div.tail-1;
	result.tail = getZeroes(result.data, result.tail);
	return result;	
}

struct Number Power(struct Number A, struct Number e_copy, struct Number N){
	struct Number P, temp, divisor, E;
	E = e_copy, temp = A;
	P.tail = 0, divisor.tail = 0;
	int  a = 0;
	strcpy(P.data, "1\0");
	strcpy(divisor.data, "2\0");
	
	a = Greater(temp, N);
	if (a == 1){ //get the modulo not the quotient
		temp = Divide(temp, N);
		temp.tail = temp.r_tail;
		strcpy(temp.data, temp.remainder);
	}
	while(E.tail >= 0){
		if((E.data[E.tail] - '0')%2 != 0){
			a = Greater(P, temp);

			if(a == 1) P = Multiply(P, temp);
			else P = Multiply(temp, P);
			
			a = Greater(P, N);
			//get the remainder when p is divided by N
			if(a == 1){
				P = Divide(P, N);
				P.tail = P.r_tail;
				strcpy(P.data, P.remainder);
			}
		}
		if(E.tail == 0 && E.data[0] == '1') break;
		E  = Divide(E, divisor);
		// printf("HOYHYOHYOYHOYHYOHYOYYHOYHYO E IS %s\n", E.data);


		temp = Multiply(temp, temp);
		a = Greater(temp, N);
		if(a == 1){
			temp = Divide(temp, N);
			temp.tail = temp.r_tail;
			strcpy(temp.data, temp.remainder);
		}
	}
	a = Greater(P, N);
	//get the remainder when p is divided by N
	if(a == 1){
		P = Divide(P, N);
		P.tail = P.r_tail;
		strcpy(P.data, P.remainder);
	}
	return P;
}

struct Number toBase27(struct Number A){
	char arr[27] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z', ' '};

	struct Number result, divisor;	
	strcpy(divisor.data, "27\0");
	divisor.tail = 1;
	
	int  rem = 0, i = 0, a=1;	
	while(A.tail >= 0){
		A = Divide(A, divisor);
		if(A.r_tail == 1) rem = ((A.remainder[0] - '0') * 10) + A.remainder[1] - '0';
		else rem = A.remainder[0] - '0';
		result.data[i] = arr[rem];

		i++;
		if (a != 1) break;
		a = Greater(A, divisor);
	}
	result.data[i] = '\0';
	result.tail = i-1;
	reverse(result.data, i-1);
	return result;


}
struct Number toBase10(struct Number message){
	struct Number mult, multiplier, total, result, str_val;
	strcpy(mult.data, "27\0"), 	strcpy(total.data, "0\0"), strcpy(multiplier.data, "1\0");
	mult.tail = 1, total.tail = 0, multiplier.tail = 0 ;
	str_val.data[2] = '\0';
	int val = 0;

	for (int i = message.tail; i >= 0; --i){
		val = findIndex(message.data[i]);
		if (val > 9){
			str_val.data[0] = val/10 + '0';
			str_val.data[1] = val%10 + '0';
			str_val.tail = 1;
		}else{
			str_val.data[0] = val + '0';
			str_val.data[1] = '\0';
			str_val.tail = 0;
		}if (val == -1)
		{
			printf("INVALID INPUT \n");
		}
		if(i != message.tail) result = Multiply(multiplier, str_val);
		else result = str_val;
		if (result.tail > total.tail)
			total = Add(result, total);
		else total = Add( total, result);		
		if (i == message.tail) multiplier = mult; 
		else multiplier = Multiply(multiplier, mult);

	}

	return total;
}


struct Number Process(struct Number A, struct Number B, char op){
	struct Number result;
	int a;
	a = Greater(A, B);

	if (op == '+' || op == '-'){
		if(op == '-'){
			if (B.sign == '+') B.sign = '-';
			else B.sign = '+';
		}
		if (A.sign == B.sign){
			if(a == 1) result = Add(A, B);
			else result = Add(B, A);
			result.sign = A.sign;
		}else{

			if(a == 1){
				result = Subtract(A, B);
				result.sign = A.sign;

			}
			else{
				result = Subtract(B, A);
				result.sign = B.sign;
			}
		}
	}
	else if (op == '*'){
		if(a == 1) result = Multiply(A, B);
		else result = Multiply(B, A);

		if(A.sign == B.sign) result.sign = '+';
		else result.sign = '-';
	}
	else if (op == '/'){
		if(a == 1) result = Divide(A, B);
		else result = Divide(B, A);
		if(A.sign == B.sign) result.sign = '+';
		else result.sign = '-';
	}

	return result;
	
}

struct Number euclid_geom(struct Number A, struct Number B, char *gcd, int gcd_tail){
	struct Number  X, Y, X_PREV, Y_PREV, Q, R, M, N, temp_A, temp_B;
	struct Number output, temp;
	int a = 0;
	temp_A =  A, temp_B = B;
	temp_A.sign = '+', temp_B.sign = '+';
	strcpy(X.data, "1\0");
	strcpy(Y.data, "0\0");
	strcpy(X_PREV.data, "0\0");
	strcpy(Y_PREV.data, "1\0");
	X_PREV.tail = 0, X_PREV.sign = '+';
	Y_PREV.tail = 0, Y_PREV.sign = '+';
	X.tail = 0, X.sign = '+';
	Y.tail = 0, Y.sign = '+';
	Q.sign = '+', R.sign = '+', M.sign = '+', N.sign = '+';
	output.sign = '+', temp.sign = '+';

	while(1){
		Q = Process(temp_B, temp_A, '/');
		strcpy(R.data, Q.remainder);
		R.tail = Q.r_tail;
		a = Greater(Q, X); 
		if (a == 1) temp = Process(Q, X, '*');
		else temp = Process(X, Q, '*');

		M = Process(X_PREV, temp, '-');
		a = Greater(Q, Y); 
		//special multiplication - with extra cheese
		if (a == 1) temp = Process(Q, Y, '*');
		else temp = Process(Y, Q, '*');
		N = Process(Y_PREV, temp, '-');
		
		X_PREV = X;
		Y_PREV = Y;
		X = M;
		Y = N;
		temp_B = temp_A;
		temp_A = R;
		if(temp_A.data[0] == '0' && temp_A.tail == 0) break;
		// printf("BESHIE DITO %c : %d\n", temp_A.data[0], temp_A.tail);
		// break;
		
	}struct Number B1;
	B1 = B;
	B1.sign = '+';
	while(X_PREV.sign != '+'){
		a = Greater(X_PREV, B1); 
		if(a == 1) X_PREV = Process(X_PREV, B1, '+');
		else X_PREV = Process(B1, X_PREV, '+');
	}
	strcpy(gcd, temp_B.data);
	gcd_tail = temp_B.tail;

	return X_PREV;
}



int main(){
	FILE *in = fopen("mp2.txt","r");
	FILE *out = fopen("201349985.txt", "w");

	char *line = malloc(400 *sizeof(char));
	fgets(line,400, in);
	fprintf(out, "%s", line);

	int l_index = 0, ind_start = 0,gcd_tail = 0;
	int send = 0, iter=0, n_iter=0;
	char marker;

	struct Number P, Q, E, N, D, PN, orig_message, proc_message;
	struct Number array[8]={P, Q, E, N, D, PN, orig_message, proc_message};
	for (int i = 0; i < 8; ++i) array[i].sign = '+';

	char *name1 = malloc(10*sizeof(char));
	char *GCD = malloc(400 *sizeof(char));

	while(!feof(in)){ // not EOF

		fgets(line, 400, in);
		
		ind_start = 0;

		for (int i = 0; i < 400; ++i){
			while(line[i] != ','){
				P.data[ind_start] = line[i];
				i++;
				ind_start++;
			}
			P.tail = ind_start-1;
			P.data[ind_start] = '\0';
			i = i + 2; // skip the space after ','
			ind_start = 0;

			while(line[i] != '\n'){
				Q.data[ind_start] = line[i];
				i++;
				ind_start++;
			}Q.tail = ind_start-1;
			Q.data[ind_start] = '\0';
			i = 400;
		}
		if(P.tail > Q.tail) N = Multiply(P, Q);
		else N = Multiply(Q, P);

		//subtract 1 from the last digits of P and Q
		P.data[P.tail] = ((P.data[P.tail] - '0')-1)+'0';
		Q.data[Q.tail] = ((Q.data[Q.tail] - '0')-1)+'0';

		if(P.tail > Q.tail) PN = Multiply(P, Q);
		else PN = Multiply(Q, P);

		fgets(E.data, 400, in);
		for (int i = 0; i < 400; ++i)
		{
			if (E.data[i] == '\n'){
				E.tail = i-1;
				E.data[i] = '\0';
				i = 401;
			}
		}

		//reading the input
		D = euclid_geom(E, PN, GCD, gcd_tail);
		char labas = '0';
		fgets(line, 400, in);
		if(gcd_tail != 0 || GCD[gcd_tail] != '1'){
			fprintf(out, "Invalid encryption key.\n");
			labas = '1';
			while(1){
				fgets(line, 400, in);
				if ((line[0] == 'C' && line[1] == 'A' && line[2] == 'S' && line[3] =='E') || (feof(in)) || (line[0] == 'E' && line[1] == 'N' && line[2] == 'D')){
					fprintf(out, "%s",line );
					break;
				}
			}
		}
		
		while(labas == '0'){
			iter = 0;
			n_iter = 0;
			//parse name
			while(line[iter] != ' '){
				name1[n_iter] = line[iter];
				iter++;
				n_iter++;
			}name1[n_iter] = '\0';
			n_iter = 0;
			iter++;
			if (line[iter] == 'r'){
				send = 0;
				fprintf(out, "%s received: ", name1);
			}else{
				send = 1;
				//write to file
				fprintf(out, "%s sent: ", name1);
			}

			while(line[iter] != ':') iter++;

			iter = iter+2;

			if (send) marker = 't';
			else marker = 'f';

			while(line[iter] != marker){
				orig_message.data[n_iter] = line[iter];
				n_iter++;
				iter++;
			}orig_message.data[n_iter-1] = '\0';
			orig_message.tail = n_iter-2;

			if (send) iter = iter + 3;
			else iter = iter + 5;
			n_iter = 0;

			//do the process here
			//base 27 -> 10

			orig_message = toBase10(orig_message);
			if(send) proc_message = Power(orig_message, E, N);
			else proc_message = Power(orig_message, D, N);

			proc_message = toBase27(proc_message);
			//write to file

			fprintf(out, "%s\n", proc_message.data);
			fgets(line, 400, in);

			if ((line[0] == 'C' && line[1] == 'A' && line[2] == 'S' && line[3] =='E') || (feof(in))){
				fprintf(out, "%s",line );
				labas = '1';
			}if (line[0] == 'E' && line[1] == 'N' && line[2] == 'D') labas = '1';
		}
		if(feof(in)) break;
		if (line[0] == 'E' && line[1] == 'N' && line[2] == 'D'){
			free(line);
			free(name1);
			free(GCD);
			break;
		}


	}

	fclose(in);
	fclose(out);
	return 0;
}
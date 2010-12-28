#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
  
#define CONST 20
#define MAX_LEN 30

double hours(int a, int b, int c, int d);
double gross(double hours, double payrate);
double taxes(double hours, double payrate);

struct employee {
  char first[MAX_LEN];
  char last[MAX_LEN];
  double payperhr;
  double gross;
  double taxes;
  double hours_in_week;
};

int main(void) {
	int n, i, j, k, m, x, y, z;
	int a, b, c, d;
	double h, g, t;
	char first[MAX_LEN], last[MAX_LEN];
	
	FILE *ifp, *ofp;
	ifp = fopen("clock.txt", "r");
	ofp = fopen("w2.txt", "w");
	fscanf(ifp, "%d", &n);
	
	struct employee prof[CONST];
	
	for (i=0; i<n; i++){
		fscanf(ifp, "%s %s %lf", prof[i].first, prof[i].last, &prof[i].payperhr);
    }
	fscanf(ifp, "%d", &k);
	
	for (i=0; i<k; i++) {
		for (x=0; x<n; x++)
				prof[x].hours_in_week = 0;
		fscanf(ifp, "%d", &m);
		for (y=0; y<m; y++) {
			fscanf(ifp, "%s %s", last, first);
			fscanf(ifp, "%d %d %d %d", &a, &b, &c, &d);
			h = hours(a, b, c, d);
			for (j=0; j<n; j++) {
				if(strcmp( first, prof[j].first) == 0 && strcmp( last, prof[j].last) == 0)
					prof[j].hours_in_week += h;
			}
		}
		for (z=0; z<n; z++) {
			g = gross(prof[z].hours_in_week, prof[z].payperhr);
			prof[z].gross += g;
			t = taxes(prof[z].hours_in_week, prof[z].payperhr);
			prof[z].taxes += t;
		}
	}

	fprintf(ofp, "Number of employees: %d\n\n", n);
	for (i=0; i<n; i++){
		fprintf(ofp, "W2 Form \n-------\n");
		fprintf(ofp, "Name: %s %s \n", prof[i].first, prof[i].last);
		fprintf(ofp, "Gross Pay: %.2lf \n", prof[i].gross);
		fprintf(ofp, "Taxes Withheld: %.2lf\n", prof[i].taxes);
		fprintf(ofp, "Internet Pay: %.2lf\n", (prof[i].gross - prof[i].taxes));
	
	}
	fclose(ifp);
	fclose(ofp);
	return 0;
}


double hours(int a, int b, int c, int d) {
	double totalhours, intime, outtime;
	intime = (a*60)+b;
	outtime = (c*60)+d;
	totalhours = ((outtime-intime)/60);

	return totalhours;
}

double gross(double hours, double payrate) {
	double total=0;
	double overtime;
	if (hours > 40) {
		total = (40*payrate);
		overtime = (hours-40);
		total += (1.5*overtime*payrate);
	}
	else
		total = (hours*payrate);
	return total;
}

double taxes(double hours, double payrate) {
	double total=0;
	double tax=0;
	double overtime;
	if (hours > 40) {
		total = (40*payrate);
		tax = (.1*total);
		overtime = (hours-40);
		total = (1.5*overtime*payrate);
		tax += (.2*total);
	}
	else {
		total = (hours*payrate);
		tax = (.1*total);
	}
	return tax;
}

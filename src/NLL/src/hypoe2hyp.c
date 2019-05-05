/*
 * Copyright (C) 1999 Anthony Lomax <lomax@faille.unice.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */


/*   hypoe2hyp.c

	Program to convert hypoellipse summary files to NLLoc .hyp format

*/

/*------------------------------------------------------------/ */
/* Anthony Lomax           | email: lomax@faille.unice.fr     / */
/* UMR Geosciences Azur    | web: www-geoazur.unice.fr/~lomax / */
/* 250 Rue Albert Einstein | tel: 33 (0) 4 93 95 43 25        / */
/* 06560 Valbonne, FRANCE  | fax: 33 (0) 4 93 65 27 17        / */
/*------------------------------------------------------------/ */


/*
	history:

	ver 01    24Nov1998  AJL  Original version


.........1.........2.........3.........4.........5.........6.........7.........8

*/



#include "GridLib.h"


/* defines */



/* globals */



/* functions */

int Hypoe2Hyp(int , char **);
int ReadHypoellSum(FILE *fp_in, HypoDesc *phypo);



/*** program to sum event scatter files */

#define PNAME  "hypoe2hyp"


int main(int argc, char *argv[])
{

	int istat, narg;


	/* set program name */

	strcpy(prog_name, PNAME);


	/* check command line for correct usage */

	fprintf(stdout, "\n%s Arguments: ", prog_name);
	for (narg = 0; narg < argc; narg++)
		fprintf(stdout, "<%s> ", argv[narg]);
	fprintf(stdout, "\n");

	if (argc < 3) {
		puterr("ERROR wrong number of command line arguments.");
		disp_usage(PNAME,
"<hypoellipse_file> <out_hyp_file> [EllLenMax [RMSMax [NRdgsMin [GapMax]]]]");
		exit(-1);
	}

	if ((istat = Hypoe2Hyp(argc, argv)) < 0) {
		puterr("ERROR converting hypoellise summary file.");
		exit(-1);
	}



	exit(0);

}



int Hypoe2Hyp(int argc, char *argv[])
{
	int istat;
	int nLocWritten, nLocRead;
	char fn_hyp_out[FILENAME_MAX];
	char fn_hypoell_in[FILENAME_MAX];
	FILE *fp_hyp_out, *fp_hypoell_in;


	double EllLenMax, RMSMax;
	int NRdgsMin, GapMax;

	GridDesc locgrid;
	HypoDesc Hypo, *phypo;



	/* get command line parameters */
	strcpy(fn_hypoell_in, argv[1]);
	strcpy(fn_hyp_out, argv[2]);

	EllLenMax = 1.0e6;
	if (argc > 3) {
		sscanf(argv[3], "%lf", &EllLenMax);
	}
	fprintf(stdout, "  Ellipsoid Len3 Maximum: %lf\n", EllLenMax);
	RMSMax = 1.0e6;
	if (argc > 4) {
		sscanf(argv[4], "%lf", &RMSMax);
	}
	fprintf(stdout, "  RMS Maximum: %lf\n", RMSMax);
	NRdgsMin = 0;
	if (argc > 5) {
		sscanf(argv[5], "%d", &NRdgsMin);
	}
	fprintf(stdout, "  Num Readings Minimum: %d\n", NRdgsMin);
	GapMax = 360;
	if (argc > 6) {
		sscanf(argv[6], "%d", &GapMax);
	}
	fprintf(stdout, "  Gap Maximum: %d\n", GapMax);



	/* open hypoellipse summary file */

	if ((fp_hypoell_in = fopen(fn_hypoell_in, "r")) == NULL) {
		puterr("ERROR: opening scatter output file.");
		return(-1);
	}

	/* open ascii hypocenter output file */
	if ((fp_hyp_out = fopen(fn_hyp_out, "w")) == NULL) {
		puterr("ERROR: opening scatter ascii output file.");
		return(-1);
	}



	nLocWritten = 0;
	nLocRead = 0;
	while (1) {

		strcpy(Hypo.fileroot, fn_hypoell_in);
		strcpy(Hypo.locStat, "Converted from hypoellipse");
		Hypo.grid_misfit_max = -1.0;

		/* read next hypocenter */
	    	if ((istat = ReadHypoellSum(fp_hypoell_in, &Hypo)) == EOF)
			break;
		else if (istat < 0) {
			puterr2(
"ERROR: reading hypoellipse summary file", fn_hypoell_in);
			break;
		}
		nLocRead++;

		if (strcmp(Hypo.locStat, "ABORTED") == 0) {
			puterr("WARNING: location ABORTED, ignoring event");
			continue;
		} else if (strcmp(Hypo.locStat, "REJECTED") == 0) {
			puterr("WARNING: location REJECTED, ignoring event");
			continue;
		} else if (Hypo.ellipsoid.len1 > EllLenMax
				|| Hypo.ellipsoid.len2 > EllLenMax
				|| Hypo.ellipsoid.len3 > EllLenMax) {
/*			puterr(
"WARNING: location ellipsoid Len is greater than EllLenMax, ignoring event");*/
			continue;
		} else if (Hypo.rms > RMSMax) {
/*			puterr(
"WARNING: location RMS is Greater than RMSMax, ignoring event");*/
			continue;
		} else if (Hypo.nreadings < NRdgsMin) {
/*			puterr(
"WARNING: location num readings is less than NRdgsMin, ignoring event");*/
			continue;
		} else if (Hypo.gap > GapMax) {
/*			puterr(
"WARNING: location gap is greater than GapMax, ignoring event");*/
			continue;
		} else {
			NumArrivals = 0;
			WriteLocation(fp_hyp_out, &Hypo,
				Arrival, NumArrivals, fn_hyp_out, 0, 0, 1,
				&locgrid, 0);
		}


		/* wirte ellipsoid */
		phypo = &Hypo;
		fprintf(fp_hyp_out,
			"ELLIPSOID  Hyp  %lf %lf %lf",
			phypo->dlat, phypo->dlong, phypo->depth);
		fprintf(fp_hyp_out,
			" Ell1  %.1lf %.1lf %.2le",
			phypo->ellipsoid.az1, phypo->ellipsoid.dip1,
				phypo->ellipsoid.len1);
		fprintf(fp_hyp_out, " Ell2  %.1lf %.1lf %.2le",
			phypo->ellipsoid.az2, phypo->ellipsoid.dip2,
				phypo->ellipsoid.len2);
		fprintf(fp_hyp_out, " Ell3  %.2le\n", phypo->ellipsoid.len3);

		/* write end line and blank line */
		fprintf(fp_hyp_out, "END_NLLOC\n\n");

		nLocWritten++;

	}


	fclose(fp_hyp_out);

	/* write message */
	fprintf(stdout,
"%d locations read, %d written to ascii hyp file <%s>\n",
		nLocRead, nLocWritten, fn_hyp_out);


	return(0);

}



/*** function to read hypoellipse summary record to HypoDesc structure */

int ReadHypoellSum(FILE *fp_in, HypoDesc *phypo)
{

	int istat;
	char *cstat;
	double mag;

	double deg, dmin;
	char strNS[2];

	static char line[MAXLINE_LONG];


	/* read next line */
	cstat = fgets(line, MAXLINE_LONG, fp_in);
 	if (cstat == NULL)
		return(EOF);

	/* read hypocenter parameters */

	istat = 0;
	istat += ReadFortranInt(line, 1, 2, &phypo->year);
	istat += ReadFortranInt(line, 3, 2, &phypo->month);
	istat += ReadFortranInt(line, 5, 2, &phypo->day);
	istat += ReadFortranInt(line, 7, 2, &phypo->hour);
	istat += ReadFortranInt(line, 9, 2, &phypo->min);
	istat += ReadFortranReal(line, 11, 4, &phypo->sec);
	phypo->sec /= 100.0;

	istat += ReadFortranReal(line, 15, 2, &deg);
	istat += ReadFortranString(line, 17, 1, strNS);
	istat += ReadFortranReal(line, 18, 4, &dmin);
	phypo->dlat = deg + dmin / 6000.0;
	if (strncmp(strNS, "S", 1) == 0)
		phypo->dlat = -phypo->dlat;
	istat += ReadFortranReal(line, 22, 3, &deg);
	istat += ReadFortranString(line, 25, 1, strNS);
	istat += ReadFortranReal(line, 26, 4, &dmin);
	phypo->dlong = deg + dmin / 6000.0;
	if (strncmp(strNS, "E", 1) != 0)
		phypo->dlong = -phypo->dlong;

	istat += ReadFortranReal(line, 30, 5, &phypo->depth);
	phypo->depth /= 100.0;

	istat += ReadFortranReal(line, 35, 2, &mag);
	mag /= 10.0;
	istat += ReadFortranInt(line, 37, 3, &phypo->nreadings);
	istat += ReadFortranInt(line, 40, 3, &phypo->gap);
	istat += ReadFortranReal(line, 43, 3, &phypo->dist);
	istat += ReadFortranReal(line, 46, 4, &phypo->rms);
	phypo->rms /= 100.0;

	istat += ReadFortranReal(line, 50, 3, &(phypo->ellipsoid.az1));
	istat += ReadFortranReal(line, 53, 2, &(phypo->ellipsoid.dip1));
	istat += ReadFortranReal(line, 55, 4, &(phypo->ellipsoid.len1));
	phypo->ellipsoid.len1 /= 100.0;

	istat += ReadFortranReal(line, 59, 3, &(phypo->ellipsoid.az2));
	istat += ReadFortranReal(line, 62, 2, &(phypo->ellipsoid.dip2));
	istat += ReadFortranReal(line, 64, 4, &(phypo->ellipsoid.len2));
	phypo->ellipsoid.len2 /= 100.0;

	istat += ReadFortranReal(line, 69, 2, &phypo->amp_mag);
	phypo->amp_mag /= 10.0;
	istat += ReadFortranReal(line, 71, 2, &phypo->dur_mag);
	phypo->dur_mag /= 10.0;

	istat += ReadFortranReal(line, 73, 4, &(phypo->ellipsoid.len3));
	phypo->ellipsoid.len3 /= 100.0;

/*	fprintf(stdout,
"%4.4d%2.2d%2.2d %2.2d%2.2d %5.2lf %3d %1c %5.2lf %4d %1c %5.2lf %7.3lf ",
		phypo->year, phypo->month, phypo->day,
		phypo->hour, phypo->min, phypo->sec,
		(int) fabs(phypo->dlat), (phypo->dlat >= 0.0 ? 'N' : 'S'),
		(fabs(phypo->dlat) - (int) fabs(phypo->dlat)) * 60.0,
		(int) fabs(phypo->dlong), (phypo->dlong >= 0.0 ? 'E' : 'W'),
		(fabs(phypo->dlong) - (int) fabs(phypo->dlong)) * 60.0,
		phypo->depth);
	fprintf(stdout, "%4.2lf %3d %3d %6.2lf %5.2lf ",
		mag, phypo->nreadings, phypo->gap, phypo->dist, phypo->rms);
	fprintf(stdout, "%4d %4d %6.2lf %4d %4d %6.2lf %6.2lf ",
		(int) (0.5 + phypo->ellipsoid.az1),
			(int) (0.5 + phypo->ellipsoid.dip1),
			phypo->ellipsoid.len1,
		(int) (0.5 + phypo->ellipsoid.az2),
			(int) (0.5 + phypo->ellipsoid.dip2),
			phypo->ellipsoid.len2,
		phypo->ellipsoid.len3);
	fprintf(stdout, "\n");
*/
	return(istat);



}


/*------------------------------------------------------------/ */
/* Anthony Lomax           | email: lomax@faille.unice.fr     / */
/* UMR Geosciences Azur    | web: www-geoazur.unice.fr/~lomax / */
/* 250 Rue Albert Einstein | tel: 33 (0) 4 93 95 43 25        / */
/* 06560 Valbonne, FRANCE  | fax: 33 (0) 4 93 65 27 17        / */
/*------------------------------------------------------------/ */


#include <Rcpp.h>
#include <assert.h>
#if 0
#include <fstream>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#include "mytime.h"
#if 0
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_sf.h>
gsl_rng *RAND_GSL;

/* Convenient macros. */
#define DEFAULT_RULESET_SIZE  3
#define DEFAULT_RULE_CARDINALITY 3
#define MAX_RULE_CARDINALITY 3
#define NLABELS 2
#endif

// extern "C" {
// // https://stackoverflow.com/questions/1793800/can-i-redefine-a-c-macro-then-define-it-back
// #pragma push_macro("__cplusplus")
// #undef __cplusplus
#include "rule.h"
// #define __cplusplus
// #pragma pop_macro("__cplusplus")

PredModel train(Data &, int, int, const Params &);
void load_data(std::string &, std::string &, Data &);
void load_data2(Data &data, Rcpp::StringVector ruleNames, Rcpp::StringVector labelNames, Rcpp::IntegerMatrix ruleTruthTables, Rcpp::IntegerMatrix labelTruthTables)
{
        data.nrules = ruleNames.size();
        data.nsamples = ruleTruthTables.ncol();
}

// #if 0
// int debug;
// #endif
// }

    Rcpp::List _train(int initialization, int method, Rcpp::List paramList, Rcpp::CharacterVector dataFile, Rcpp::CharacterVector labelFile,
                      Rcpp::StringVector ruleNames, Rcpp::StringVector labelNames, Rcpp::IntegerMatrix ruleTruthTables, Rcpp::IntegerMatrix labelTruthTables) {
//        Rprintf("training!\n");
	Data	data, data2;
	struct timeval tv_acc, tv_start, tv_end;
	std::string df = Rcpp::as<std::string>(dataFile[0]);
	std::string lf = Rcpp::as<std::string>(labelFile[0]);
        Rcpp::IntegerVector id;
        Rcpp::NumericVector prob;

        try {
                /*
                * We treat the label file as a separate ruleset, since it has
                * a similar format.
                */
                INIT_TIME(tv_acc);
                START_TIME(tv_start);
                data.nrules = ruleNames.size(); // will increment to add the default rule that's not included in the matrix
                data.nsamples = ruleTruthTables.ncol();
                load_data(df, lf, data);
                END_TIME(tv_start, tv_end, tv_acc);
                REPORT_TIME("Initialize time", "per rule", tv_end, data.nrules);

                load_data2(data2, ruleNames, labelNames, ruleTruthTables, labelTruthTables);
        //#if 0
        //        if (debug)
        //                printf("%d rules %d samples\n\n", nrules, nsamples);
        //
        //        if (debug > 100)
        //                rule_print_all(rules, nrules, nsamples);
        //
        //        if (debug > 100) {
        //                printf("Labels for %d samples\n\n", nsamples);
        //                rule_print_all(labels, nsamples, nsamples);
        //        }
        //#endif

                Params params;
                Rcpp::NumericVector nv;
                Rcpp::IntegerVector iv;
                nv = paramList[0];
                params.lambda = nv[0];
                nv = paramList[1];
                params.eta = nv[0];
                nv = paramList[2];
                params.threshold = nv[0];
                nv = paramList[3];
                params.alpha[0] = nv[0];
                params.alpha[1] = nv[1];
                iv = paramList[4];
                params.iters = iv[0];
                iv = paramList[5];
                params.nchain = iv[0];

                INIT_TIME(tv_acc);
                START_TIME(tv_start);
                PredModel pred_model_sbrl = train(data, initialization, method, params);
                END_TIME(tv_start, tv_end, tv_acc);
                REPORT_TIME("Time to train", "", tv_end, 1);


                for (int i=0; i<pred_model_sbrl.ids.size(); i++)
                        id.push_back(pred_model_sbrl.ids[i]);

                for (int i=0; i<pred_model_sbrl.thetas.size(); i++)
                        prob.push_back(pred_model_sbrl.thetas[i]);

        #if 0
                Rcpp::NumericVector ci_low;
                for (int i=0; i<pred_model_brl->rs->n_rules; i++)
                        ci_low.push_back(pred_model_brl->confIntervals->a);

                Rcpp::NumericVector ci_high;
                for (int i=0; i<pred_model_brl->rs->n_rules; i++)
                        ci_high.push_back(pred_model_brl->confIntervals->b);
        #endif
        }
        catch (const std::overflow_error& e) {
                Rprintf("overflow_error: %s\n", e.what());
        }
        catch (const std::runtime_error& e) {
                Rprintf("runtime_error: %s\n", e.what());
        }
        catch (const std::exception& e) {
                Rprintf("exception: %s\n", e.what());
        }
        catch (...) {
                Rprintf("unknown error. please investigate or contact the maintainer of the package.\n");
        }
        // Rcpp::DataFrame brl =  Rcpp::DataFrame::create(Rcpp::Named("clause")=clause, Rcpp::Named("prob")=prob, Rcpp::Named("ci_low")=ci_low, Rcpp::Named("ci_high")=ci_high);
        Rcpp::DataFrame rs =  Rcpp::DataFrame::create(Rcpp::Named("V1")=id, Rcpp::Named("V2")=prob);

        return(Rcpp::List::create(Rcpp::Named("rs")=rs));
    }

//using namespace Rcpp;

// Rcpp::List _train(int initialization, int method, Rcpp::List paramList, Rcpp::CharacterVector dataFile, Rcpp::CharacterVector labelFile)
// fastLR_
// Rcpp::List fastLR_(Rcpp::NumericMatrix x, Rcpp::NumericVector y, Rcpp::NumericVector start, double eps_f, double eps_g, int maxit);
RcppExport SEXP sbrl_train(SEXP initSEXP, SEXP methodSEXP, SEXP paramListSEXP, SEXP dataFileSEXP, SEXP labelFileSEXP,
                           SEXP ruleNamesSEXP, SEXP labelNamesSEXP, SEXP ruleTruthTablesSEXP, SEXP labelTruthTablesSEXP) {
    BEGIN_RCPP
    Rcpp::traits::input_parameter< int >::type init(initSEXP);
    Rcpp::traits::input_parameter< int >::type method(methodSEXP);
    Rcpp::traits::input_parameter< Rcpp::List >::type params(paramListSEXP);
    Rcpp::traits::input_parameter< Rcpp::CharacterVector >::type dataFile(dataFileSEXP);
    Rcpp::traits::input_parameter< Rcpp::CharacterVector >::type labelFile(labelFileSEXP);
    Rcpp::traits::input_parameter< Rcpp::StringVector >::type ruleNames(ruleNamesSEXP);
    Rcpp::traits::input_parameter< Rcpp::StringVector >::type labelNames(labelNamesSEXP);
    Rcpp::traits::input_parameter< Rcpp::IntegerMatrix >::type ruleTruthTables(ruleTruthTablesSEXP);
    Rcpp::traits::input_parameter< Rcpp::IntegerMatrix >::type labelTruthTables(labelTruthTablesSEXP);
    //__result = Rcpp::wrap(_train(x, y, start, eps_f, eps_g, maxit));
    //return __result;
    return Rcpp::wrap(_train(init, method, params, dataFile, labelFile, ruleNames, labelNames, ruleTruthTables, labelTruthTables));
    END_RCPP
}
// Fortran code and Found no calls to: 'R_registerRoutines', 'R_useDynamicSymbols'
// https://stackoverflow.com/questions/43101032/fortran-code-and-found-no-calls-to-r-registerroutines-r-usedynamicsymbols
#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <R_ext/Rdynload.h>
#include <Rcpp.h>

/* FIXME:
Check these declarations against the C/Fortran source code.
*/

// extern void F77_NAME(cf)(int *r, int *cd, double *loci);

static const R_FortranMethodDef FortranEntries[] = {
  {"sbrl_train", (DL_FUNC) &sbrl_train,  3},
  {NULL, NULL, 0}
};

void R_init_sbrl(DllInfo *dll)
{
  R_registerRoutines(dll, NULL, NULL, FortranEntries, NULL);
  R_useDynamicSymbols(dll, FALSE);
}

void
load_data(std::string &data_file, std::string &label_file, Data &data)
{
        int add_default_rule;

        /* Load data. */
        add_default_rule = 1;
        data.nrules++;
        data.rules.reserve(data.nrules);
        rules_init(data_file, data.rules, data.nrules, data.nsamples, add_default_rule);

        /* Load labels. */
        add_default_rule = 0;
        data.labels.reserve(2);
        rules_init(label_file, data.labels, 2, data.nsamples, add_default_rule);
}

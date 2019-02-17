#ifndef NETWORK_H
#define NETWORK_H
#include <fstream>
#include <string>
#include <armadillo>
#include <cmath>
#include <vector>

using namespace arma;

// qt includes
#include <QDebug>


class network
{
public:
    network(uint n_features, uint n_hidden, uint n_output);
    void load(std::string filename);
    void save_results();
    void restore();
    int predict(Mat<double> x);
    void count_score();
    void SGD(uint epochs, double eta);
    void MBGD(uint epochs, uint batch, double eta,
              double decr_cost, double momentum);

    Mat<double> inputs;
    Mat<double> labels;


private:
    uint n_features;
    uint n_hidden;
    uint n_output;



    Mat<double> prepare_labels(Mat<double> label);

    void init_weights();
    Mat<double> W1;
    Mat<double> W2;

    double sigmoid(double X);
    double sigmoid_der(double X);

    void evaluate(Mat<double> x, Mat<double> &z1, Mat<double> &z2, Mat<double> &a1, Mat<double> &a2);
    void backprop(Mat<double> &g1, Mat<double> &g2, Mat<double> y, Mat<double> x);
    void add_bias_unit(Mat<double> &layer, std::string method);
    double error(Mat<double> output, Mat<double> y);
    void split_array();

};


#endif // NETWORK_H

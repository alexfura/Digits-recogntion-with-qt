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
    network(int n_output, int n_hidden, int epoch,
            uint batch_size, double eta, double decreasing_cost);
    void load(std::string filename);
    void save_results();
    void fit();
    void restore();
    int predict(Mat<double> x);
    void test_train(std::string filename);


private:
    int n_output;
    int n_hidden;
    int epochs;
    uint batch_size;
    double eta;
    double decreasing_cost;

    void evaluate(Mat<double> x, Mat<double> &z1, Mat<double> &z2, Mat<double> &a1, Mat<double> &a2);
    void backprop(Mat<double> &g1, Mat<double> &g2, Mat<double> z1, Mat<double> z2, Mat<double> a1, Mat<double> a2, Mat<double> o, Mat<double> x);

    Mat<double> inputs;
    Mat<double> labels;
    void init_weights();
    Mat<double> W1;
    Mat<double> W2;
    double sigmoid(double X);
    Mat<double> prepare_labels(Mat<double> label);
    double sigmoid_der(double X);

    void SGD();
    void add_bias_unit(Mat<double> &layer, std::string method);
    double error(Mat<double> output, Mat<double> y);
    void MBGD();
    void split_array();

};


#endif // NETWORK_H

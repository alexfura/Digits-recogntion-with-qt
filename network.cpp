#include "network.h"


network::network(int n_output, int n_hidden, int epoch,
                 uint batch_size, double eta, double decreasing_cost)
{
    this->n_output = n_output;
    this->n_hidden = n_hidden;
    this->epochs = epoch;
    this->batch_size = batch_size;
    this->eta = eta;
    this->decreasing_cost = decreasing_cost;

    // fill weights with random
    init_weights();
    // encode labels
}

void network::load(std::string filename)
{
    // load unprepared data

    mat Raw = Mat<double>();
    Raw.load(filename);

    // slice Raw matrix to inputs and labels

    this->inputs = Raw.submat(0, 1, Raw.n_rows-1, Raw.n_cols-1);
    this->labels = Raw.submat(0, 0, Raw.n_rows-1, 0);
    this->inputs = this->inputs / 255;
    this->inputs.for_each([](mat::elem_type& val){
        if(val > 0.5)
        {
            val = 1;
        }
        else
        {
            val  = 0;
        }
    });
    this->labels = prepare_labels(this->labels);
}


void  network::init_weights()
{
    arma_rng::set_seed_random();
    // [-1 .... 1]
    this-> W1 = this->W1.randu(this->n_hidden, 784);
//    qDebug() <<W1.n_rows <<W1.n_cols;
    this->W2 = this->W2.randu(this->n_output, this->n_hidden +1);
//    qDebug() <<W2.n_rows <<W2.n_cols;
}

void network::evaluate(Mat<double> x, Mat<double> &z1,
                              Mat<double> &z2, Mat<double> &a1, Mat<double> &a2)
{
    z1 = x * this->W1.t();
    // 1x30

    a1 = z1.for_each([](mat::elem_type& val){val=1/(1+exp(-val));});

    this->add_bias_unit(a1, "column");
    // 1x30 to 1 31

    z2 = a1 * this->W2.t();
    // 1x31 * 31x10 -> 1x10

    a2 = z2.for_each([](mat::elem_type& val){val=1/(1+exp(-val));});
    // applying activation derivative to output from hidden layer
    // 1x10
}


void network::fit()
{

}

void network::SGD()
{
    Mat<double> s1, s2, z1, z2, a1, a2, g1, g2, z2_der, error;

    for(uint i = 0;i < this->inputs.n_rows;i++)
    {
        this->evaluate(this->inputs.row(i), z1, z2, a1, a2);

        error = a2 - this->labels.row(i);
        qDebug() <<"Total error: "<<this->error(a2, this->labels.row(i));

        // add to z1 bias unit

        z2_der = z2.for_each([this](mat::elem_type& val){val = this->sigmoid_der(val);});

        s2 = error % z2_der;

        add_bias_unit(z1, "column");

        s1 = (this->W2.t() * error.t()) % z1.for_each([this](mat::elem_type& val){val = this->sigmoid_der(val);}).t();
        s1.t();
        s1.reshape(s1.n_rows-1, s1.n_cols);

        g2 = s2.t() * a1;
        g1 = s1 * this->inputs.row(i);


        this->W1 -= this->eta * g1;
        this->W2 -= this->eta * g2;

    }
}

void network::add_bias_unit(Mat<double>& layer, std::string method)
{
    if(method=="column")
    {
        // 1x30 -> 1x31 (item at column 30 set to 1)
        layer.resize(layer.n_rows, layer.n_cols+1);
        for(uint row = 0;row < layer.n_rows;row++)
        {
            layer.at(row, layer.n_cols-1) = 1;
        }
    }
    else if(method=="row")
    {
        layer.resize(layer.n_rows+1, layer.n_cols);
        for(uint col = 0;col < layer.n_cols;col++)
        {
            layer.at(layer.n_rows-1, col) = 1;
        }
    }
}

double network::sigmoid_der(double X)
{
    return this->sigmoid(X) * (1 - this->sigmoid(X));
}

double network::sigmoid(double X)
{
    return 1/(1 + exp(-X));
}

Mat<double> network::prepare_labels(Mat<double> label)
{
    // encode lables
    // n_rows x 1

    mat encoded = Mat<double>(this->labels.n_rows, 10).zeros();

    for(uint i = 0;i < label.n_rows;i++)
    {
        encoded.at(i, label.at(i,0)) = 1;
    }
    return encoded;
}


double network::error(Mat<double> output, Mat<double> y)
{
    double diff = accu(output - y);

    return pow(diff, 2)/2;
}

void network::MBGD()
{
    double rate = 0;
    Mat<double> z1, a1, z2, a2, g1, g2, x, y, s1, s2;

    arma_rng::set_seed_random();

    for (int epoch = 0;epoch < this->epochs;epoch++)
    {
        this->eta /= 1 + this->decreasing_cost * epoch;
        this->inputs = shuffle(this->inputs);

        qDebug() <<"Epoch #"<<epoch;
        for(uint i = 0;i < this->inputs.n_rows - this->batch_size;i+= this->batch_size)
        {
            x = this->inputs.submat(i, 0, i+this->batch_size, this->inputs.n_cols-1);
            y = this->labels.submat(i, 0, i+this->batch_size, this->labels.n_cols-1);
            evaluate(x, z1, z2, a1, a2);
            Mat<double> error = a2 - y;
            Mat<double> z2_der = z2.for_each([this](mat::elem_type& val)
            {val = this->sigmoid_der(val);});
            s2 = error % z2_der;

            add_bias_unit(z1, "column");

            s1 = (this->W2.t() * s2.t()) % z1.for_each([this](mat::elem_type& val)
            {val = this->sigmoid_der(val);}).t();
            s1.t();
            s1.reshape(s1.n_rows-1, s1.n_cols);

            g2 = s2.t() * a1;
            g1 = s1 * x;

            this->W1 -= this->eta * g1;
            this->W2 -= this->eta * g2;
        }
    }
}


int network::predict(Mat<double> x)
{
    // y - label(number), x - features vector
    Mat<double> z1, z2, a1, a2;
    this->evaluate(x, z1, z2, a1, a2);

    // a2 - output vector

    return  a2.index_max();
}

void network::test_train(std::string filename)
{
    Mat<double> Dt,x, y, z1, z2, a1, a2;
    Dt.load(filename);

    x = Dt.submat(0, 1, Dt.n_rows-1, Dt.n_cols-1);
    y = Dt.submat(0, 0, Dt.n_rows-1, 0);
    x = x / 255;

    x.for_each([](mat::elem_type& val){
        if(val > 0.5)
        {
            val = 1;
        }
        else
        {
            val  = 0;
        }
    });


    uint prediction = 0;

    double rate = 0;

    for(uint i = 0;i < x.n_rows;i++)
    {
        this->evaluate(x.row(i), z1, z2, a1, a2);
        prediction = this->predict(x.row(i));

        if(y.at(i, 0) == prediction)
        {
            rate++;
        }
    }
//    qDebug() <<"!!!!" << rate / y.n_rows <<"!!! - TOTAL %";
}


void network::save_results()
{
    this->W1.save(hdf5_name("w1.h5"));
    this->W2.save(hdf5_name("w2.h5"));
}

void network::restore()
{
    this->W1.load(hdf5_name("w1.h5"));
    this->W2.load(hdf5_name("w2.h5"));
}






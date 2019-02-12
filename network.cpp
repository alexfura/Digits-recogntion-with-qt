#include "network.h"


network::network(uint n_features,uint n_hidden, uint n_output)
{
    // [784, 100, 10]
    this->n_features = n_features;
    this->n_hidden = n_hidden;
    this->n_output = n_output;

    // fill weights with random
    init_weights();
    // encode labels
}

void network::load(std::string filename)
{
    // load unprepared data

    mat RAW_DATA = Mat<double>();
    RAW_DATA.load(filename);

    // expects, that y-label is first column

    // slice Raw matrix to inputs and labels

    this->inputs = RAW_DATA.submat(0, 1, RAW_DATA.n_rows-1, RAW_DATA.n_cols-1);
    this->labels = RAW_DATA.submat(0, 0, RAW_DATA.n_rows-1, 0);

    // make dataset more suitable for out app

    this->inputs.for_each([](mat::elem_type& val){
        if(val > 0)
        {
            val = 1;
        }
    });
    this->labels = prepare_labels(this->labels);
}


void  network::init_weights()
{
    arma_rng::set_seed_random();
    // [-1 .... 1]
    this-> W1 = this->W1.randu(this->n_hidden, this->n_features);
    // +1 with bias unit
    this->W2 = this->W2.randu(this->n_output, this->n_hidden +1);
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

void network::SGD(uint epochs, double eta)
{
    Mat<double> s1, s2, z1, z2, a1, a2, g1, g2, z2_der, error;

    for(uint i = 0;i < epochs;i++)
    {
        this->evaluate(this->inputs.row(i), z1, z2, a1, a2);

        qDebug() <<"Error: " <<this->error(a2, this->labels.row(i));
        qDebug() <<"Predicted: " << a2.index_max() << " Actual: " <<this->labels.row(i).index_max();

        error = a2 - this->labels.row(i);

        z2_der = z2.for_each([this](mat::elem_type& val){val = this->sigmoid_der(val);});

        s2 = error % z2_der;

        add_bias_unit(z1, "column");

        s1 = (this->W2.t() * error.t()) % z1.for_each([this](mat::elem_type& val){val = this->sigmoid_der(val);}).t();
        s1.t();
        s1.reshape(s1.n_rows-1, s1.n_cols);

        g2 = s2.t() * a1;
        g1 = s1 * this->inputs.row(i);


        this->W1 -= eta * g1;
        this->W2 -= eta * g2;
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


void network::MBGD(uint epochs, uint batch, double eta,
                   double decr_cost, double momentum)
{
    Mat<double> s1, s2, z1, z2, a1, a2, g1, g2, z2_der, error, x, y, v1, v2, v1_prev, v2_prev;

    v1 = zeros(this->W1.n_rows, this->W1.n_cols);
    v2 = zeros(this->W2.n_rows, this->W2.n_cols);
    v1_prev = zeros(this->W1.n_rows, this->W1.n_cols);
    v2_prev = zeros(this->W2.n_rows, this->W2.n_cols);


    for(uint i = 0;i < epochs;i++)
    {
        qDebug() <<"Epoch #" <<i;
        for (uint mini = 0;mini < this->inputs.n_rows - batch;mini += batch)
        {
            x = this->inputs.submat(mini, 0, mini + batch - 1, this->inputs.n_cols - 1);
            y = this->labels.submat(mini, 0, mini + batch - 1, this->labels.n_cols - 1);

            this->evaluate(x, z1, z2, a1, a2);

            error = a2 - y;

            z2_der = z2.for_each([this](mat::elem_type& val){val = this->sigmoid_der(val);});

            s2 = error % z2_der;

            add_bias_unit(z1, "column");

            s1 = (this->W2.t() * error.t()) % z1.for_each([this](mat::elem_type& val){val = this->sigmoid_der(val);}).t();
            s1.t();
            s1.reshape(s1.n_rows-1, s1.n_cols);

            g2 = s2.t() * a1;
            g1 = s1 * x;


            v1 = momentum * v1 - eta * g1;
            v2 = momentum * v2 - eta * g2;

            this->W1 -= v1;
            this->W2 -= v2;
        }
    }
}


int network::predict(Mat<double> x)
{
    // y - label(number), x - features vector
    Mat<double> z1, z2, a1, a2;
    this->evaluate(x, z1, z2, a1, a2);

    return  a2.index_max();
}

void network::count_score()
{
    Mat<double> z1, z2, a1, a2;

    double rate = 0;

    for(uint i = 0;i < this->labels.n_rows;i++)
    {
        if(this->predict(this->inputs.row(i)) == this->labels.row(i).index_max())
        {
            rate++;
        }

        qDebug() <<this->predict(this->inputs.row(i)) << this->labels.row(i).index_max() <<
                  " Predicted and actual";
    }

    qDebug() <<rate / this->labels.n_rows << " :Total %";

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






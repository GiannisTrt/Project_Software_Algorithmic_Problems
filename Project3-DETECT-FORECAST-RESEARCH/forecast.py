import argparse
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from tensorflow.keras import Sequential
from tensorflow.keras.layers import LSTM, Dropout, Dense
from sklearn.preprocessing import MinMaxScaler
from sklearn.metrics import mean_squared_error
from tensorflow.keras.callbacks import EarlyStopping

# Auxiliary function that creates a Multilayer Recurrent LSTM
def create_model(X_train):
    model = Sequential(
        [
            # First LSTM layer + Dropout regularization
            LSTM(units = 50, return_sequences = True, input_shape = (X_train.shape[1], 1)),
            #Dropout(0.2),
            # Second LSTM layer + Dropout regularization
            LSTM(units = 50, return_sequences = True),
            #Dropout(0.2),
            # Third LSTM layer + Dropout regularization
            LSTM(units = 50, return_sequences = True),
            Dropout(0.2),
            # Forth LSTM layer + Dropout regularization
            LSTM(units = 50),
            #Dropout(0.2),
            # Output layer
            Dense(units = 1)
        ]
    )

    return model


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-d", type=str, help="path to dataset")
    parser.add_argument("-n", type=str, help="number of time series selected")
    args = parser.parse_args()

    return args


def plot_results(test_set, predicted_stock_price):
    time_units = np.arange(test_set.shape[0])

    plt.plot(time_units, test_set[:, 0], color = 'blue', label = 'Real Stock Price')
    plt.plot(time_units, predicted_stock_price, color = 'orange', label = 'Predicted Stock Price')
    #plt.xticks(np.arange(0,459,50))
    plt.title('Stock Price Prediction')
    plt.xlabel('Time')
    plt.ylabel('Stock Price')
    plt.legend()
    plt.show()


def main():
    opt = parse_args()
    df = pd.read_csv(opt.d, header=None, sep='\t')
    print(f"Number of rows and columns: {df.shape}\n")

    split_index = int(len(df.columns)*0.8)

    training_set = df.iloc[1, 1:split_index].values
    training_set = np.reshape(training_set, (len(training_set), 1)) # shape (2919, 1)

    test_set = df.iloc[0, split_index:].values
    test_set = np.reshape(test_set, (-1, 1)) # shape (731, 1)

    TIME_STEP = 30

    normalizer = MinMaxScaler()

    # Prepare the Train Set
    training_set_scaled = normalizer.fit_transform(training_set)

    X_train = []
    y_train = []
    for i in range(TIME_STEP, split_index-1):
        X_train.append(training_set_scaled[i-TIME_STEP:i, 0])
        y_train.append(training_set_scaled[i, 0])
    X_train, y_train = np.array(X_train), np.array(y_train)
    X_train = np.reshape(X_train, (X_train.shape[0], X_train.shape[1], 1)) # shape: (2889, 30, 1) for an 80% split

    # Create, Compile and Fit an RNN model
    model = create_model(X_train)
    model.compile(optimizer = 'sgd', loss = 'mean_squared_error') # Compile the model
    history = model.fit(X_train, y_train, epochs = 30, batch_size = 32, validation_split=0.3,
    callbacks=[EarlyStopping(monitor='val_loss', patience=5, mode='min')]) # Fit the model on the training data

    plt.plot(history.history['loss'], label='Training loss')
    plt.plot(history.history['val_loss'], label='Validation loss')
    plt.legend()
    plt.show()

    # Prepare the Test Set
    dataset_total = np.concatenate((training_set, test_set), axis = 0)
    testing_set = dataset_total[-len(test_set)-TIME_STEP:]

    testing_set_scaled = normalizer.transform(testing_set)
    X_test = []
    for i in range(TIME_STEP, len(testing_set_scaled)):
        X_test.append(testing_set_scaled[i-TIME_STEP:i, 0])
    X_test = np.array(X_test)
    X_test = np.reshape(X_test, (X_test.shape[0], X_test.shape[1], 1)) # shape: (761, 30, 1)

    predicted_stock_price = model.predict(X_test)
    predicted_stock_price = normalizer.inverse_transform(predicted_stock_price)

    plot_results(test_set, predicted_stock_price)


if __name__=='__main__': main()
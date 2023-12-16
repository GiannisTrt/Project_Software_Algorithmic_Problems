import pandas as pd
import numpy as np
from sklearn.preprocessing import StandardScaler
from tensorflow.keras.layers import Dense, TimeDistributed, Dropout, LSTM, RepeatVector
from keras.callbacks import EarlyStopping
from tensorflow.keras import Sequential
import matplotlib.pyplot as plt

N = 20
SPLIT_RATIO = 0.8
TIME_STEPS = 30

#opt = parse_args()

df = pd.read_csv('nasdaq2007_17.csv', header=None, sep='\t') # Shape: (359, 3651)
df = df[:N] # Choose the first N series
df = df.loc[:, 1:] # Drop the first column (contains letters)

train_size = int(df.shape[1] * SPLIT_RATIO)
# Split and transpose the dataframes to fit into the scaler
train = df.iloc[:, 0:train_size].transpose() # Shape: (2920, N)
test = df.iloc[:, train_size:].transpose() # Shape: (730, N)

train = train.loc[:, 0] # Just take 1 series
train = np.expand_dims(train, axis=-1)
test = test.loc[:, 0] # Just take 1 series
test = np.expand_dims(test, axis=-1)

scaler = StandardScaler()
scaler = scaler.fit(train) # Fit the scaler on the train data

train_transformed = scaler.transform(train) # Transform the train data and transpose the matrix
test_transformed = scaler.transform(test) # Transform the test data and transpose the matrix

"""train_transformed = train_transformed.T
test_transformed = test_transformed.T
print(train_transformed.shape, test_transformed.shape)"""

"""X_train, y_train = [], []
for i in range(len(train_transformed)):
    temp_X_train, temp_y_train = [], []
    for j in range(train_transformed.shape[1] - TIME_STEPS):
        temp_X_train.append(train_transformed[i, j:(j + TIME_STEPS)])
        temp_y_train.append(train_transformed[i:, j + TIME_STEPS])
    #temp_X_train, temp_y_train = np.array(temp_X_train), np.array(temp_y_train)
    X_train.append(temp_X_train)
    y_train.append(temp_y_train)
X_train, y_train = np.asarray(X_train), np.asarray(y_train)
X_train = np.expand_dims(X_train, axis=-1)

print(X_train.shape, y_train.shape)

## CHANGE IT HERE AGAIN AND MAKE A FOR LOOP ##
X_train, y_train = X_train[0], y_train[0, 0]
###############################################"""

# Create the Train Set
X_train, y_train = [], []
for i in range(len(train_transformed) - TIME_STEPS):
        X_train.append(train_transformed[i:(i + TIME_STEPS)])
        y_train.append(train_transformed[i + TIME_STEPS])
X_train, y_train = np.asarray(X_train), np.asarray(y_train)

print(X_train.shape, y_train.shape)

# Create the Test Set
X_test, y_test = [], []
for i in range(len(test_transformed) - TIME_STEPS):
        X_test.append(test_transformed[i:(i + TIME_STEPS)])
        y_test.append(test_transformed[i + TIME_STEPS])
X_test, y_test = np.asarray(X_test), np.asarray(y_test)

print(X_test.shape, y_test.shape)


# Creation of the LSTM Autoencoder
model = Sequential()
model.add(LSTM(units=128, input_shape=(X_train.shape[1], X_train.shape[2])))
model.add(Dropout(rate=0.2))
model.add(RepeatVector(n=X_train.shape[1]))
model.add(LSTM(units=128, return_sequences=True))
model.add(Dropout(rate=0.2))
model.add(TimeDistributed(Dense(units=X_train.shape[2])))
#model.summary()

# Compile the model
model.compile(loss='mae', optimizer='adam')
history = model.fit(
    X_train, y_train,
    epochs=20,
    batch_size=128,
    validation_split=0.3,
    callbacks=[EarlyStopping(monitor='val_loss', patience=8, mode='min')],
    shuffle=False
)

# Plot the learning curves
plt.plot(history.history['loss'], label='Training loss')
plt.plot(history.history['val_loss'], label='Validation loss')
plt.legend()
plt.show()

# Predict the X_train samples so as to calculate the mae loss
X_train_pred = model.predict(X_train, verbose=0)
train_mae_loss = np.mean(np.abs(X_train_pred - X_train), axis=1)

# Plot the train mae and the test mae
plt.subplot(1, 2, 1)
plt.hist(train_mae_loss, bins=150)
plt.xlabel('Train MAE loss')
plt.ylabel('Number of Samples')

threshold = np.max(train_mae_loss)
print(f'Reconstruction error threshold: {threshold}')

X_test_pred = model.predict(X_test, verbose=0)
test_mae_loss = np.mean(np.abs(X_test_pred-X_test), axis=1)

plt.subplot(1, 2, 2)
plt.hist(test_mae_loss, bins=150)
plt.xlabel('Test MAE loss')
plt.ylabel('Number of samples')
plt.show()

# Create a DataFrame to save the relevant info
test_score_df = pd.DataFrame(test_transformed[TIME_STEPS:])
test_score_df['loss'] = test_mae_loss
test_score_df['threshold'] = threshold
test_score_df['anomaly'] = test_score_df['loss'] > test_score_df['threshold']
test_score_df['close'] = test_transformed[TIME_STEPS:]
test_score_df['date'] = pd.date_range(start='1/1/1986', periods=test_transformed[TIME_STEPS:].shape[0]) # Create artificial dates

plt.plot(test_score_df['date'], test_score_df['loss'], label='Test loss')
plt.plot(test_score_df['date'], test_score_df['threshold'], label='Threshold')
plt.legend()
plt.show()

anomalies = test_score_df.loc[test_score_df['anomaly'] == True]
print(anomalies.shape)

plt.plot(test_score_df['date'], scaler.inverse_transform(np.expand_dims(test_score_df['close'], axis=-1)), label='Close price')
plt.scatter(anomalies['date'], scaler.inverse_transform(np.expand_dims(anomalies['close'], axis=-1)), marker='.', c="red", label='Anomaly')
plt.legend(title='Detected anomalies')
plt.show()
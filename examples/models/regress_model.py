import tensorflow as tf

class RegressModel(tf.Module):
    @tf.function(input_signature=[tf.TensorSpec(shape=[None], dtype=tf.string)])
    # We've specifically named the arg as "inputs" to match the "classify"
    # SignatureDef expectations: https://github.com/tensorflow/tensorflow/blob/86c79e9779a2d2653fddbb0e0391e2ea8121e81c/tensorflow/cc/saved_model/signature_constants.h#L57
    def func(self, inputs):
        # https://www.tensorflow.org/api_docs/python/tf/io/parse_example#we_can_use_arguments
        # We expect a batch of examples with feature name 'price_today' which corresponds
        # to the price of a toy today. Our regression model predicts the price of the same
        # object in one year.
        example = tf.io.parse_example(inputs, {
            'price_today' : tf.io.FixedLenFeature((), tf.float32, default_value=tf.zeros(()))
        })
        # Here's an example of a terrible price prediction algorithm: 2x + 3
        price_in_one_year = tf.add(example['price_today'] * tf.constant(2.0), tf.constant(3.0))

        # Use "outputs" as the key to match the "Regress" output SignatureDef expectation
        # https://github.com/tensorflow/tensorflow/blob/86c79e9779a2d2653fddbb0e0391e2ea8121e81c/tensorflow/cc/saved_model/signature_constants.h#L63
        return {
            'outputs': price_in_one_year
        }


if __name__ == '__main__':
    model = RegressModel()
    # https://www.tensorflow.org/tutorials/load_data/tfrecord#creating_a_tftrainexample_message
    example_1_proto = tf.train.Example(
        features = tf.train.Features(feature={
            'price_today': tf.train.Feature(float_list=tf.train.FloatList(value=[tf.constant(3.0)]))
        })
    )
    example_1_string = example_1_proto.SerializeToString()

    example_2_proto = tf.train.Example(
        features = tf.train.Features(feature={
            'price_today': tf.train.Feature(float_list=tf.train.FloatList(value=[tf.constant(5.0)]))
        })
    )
    example_2_string = example_2_proto.SerializeToString()

    example_batch = tf.constant([example_1_string, example_2_string])

    result = model.func(example_batch)
    # This should say [ 9.0, 13.0 ]
    print(result)

    tf.saved_model.save(model, 'models/regress_model/1', signatures={
        'regress_signature': model.func, 
    })

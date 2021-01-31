import tensorflow as tf

class ClassifyModel(tf.Module):
    @tf.function(input_signature=[tf.TensorSpec(shape=[], dtype=tf.string)])
    # We've specifically named the arg as "inputs" to match the "classify"
    # SignatureDef expectations: https://github.com/tensorflow/tensorflow/blob/86c79e9779a2d2653fddbb0e0391e2ea8121e81c/tensorflow/cc/saved_model/signature_constants.h#L30
    def func(self, inputs):
        # https://www.tensorflow.org/api_docs/python/tf/io/parse_example#we_can_use_arguments
        # Inputs is guaranteed to be rank 0, since our input signature uses tensorspec with shape [].
        # We expect a single example with feature name 'picture' corresponding to a flattened 28 x 28 
        # float32 matrix.
        example = tf.io.parse_example(inputs, {
            'picture' : tf.io.FixedLenFeature((28*28,), tf.float32, default_value=tf.zeros(28*28,))
        })
        # Here's an example of a terrible hotdog classifier:
        # If the sum of all the values in the matrix > 100, we return "HotDog", 
        # Otherwise we return "Not HotDog".
        picture_sum = tf.math.reduce_sum(example['picture'])
        if picture_sum > tf.constant(100.0):
            result = tf.constant('HotDog')
        else:
            result = tf.constant('Not HotDog')

        # Use "Classes" as the key to match the "Classify" output SignatureDef expectation
        return {
            'classes': result
        }

if __name__ == '__main__':
    model = ClassifyModel()
    # https://www.tensorflow.org/tutorials/load_data/tfrecord#creating_a_tftrainexample_message
    example_hotdog_proto = tf.train.Example(
        features = tf.train.Features(feature={
            'picture': tf.train.Feature(float_list=tf.train.FloatList(value=tf.ones((28*28,))))
        })
    )
    example_hotdog_proto_tensor = tf.constant(example_hotdog_proto.SerializeToString())
    result = model.func(example_hotdog_proto_tensor)
    # This should say "HotDog"
    print(result)

    tf.saved_model.save(model, 'models/classify_model/1', signatures={
        'classify_signature': model.func, 
    })

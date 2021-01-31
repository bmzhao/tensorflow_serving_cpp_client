import tensorflow as tf

class ClassifyModel(tf.Module):
    @tf.function(input_signature=[tf.TensorSpec(shape=[None], dtype=tf.string)])
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
        picture_sum = tf.math.reduce_sum(example['picture'], axis=1)

        @tf.function
        def classify_single(picture_sum):
            return tf.cond(tf.greater(picture_sum, 100.0), 
                lambda: tf.constant(['HotDog']),
                lambda: tf.constant(['Not Hotdog']))

        result = tf.map_fn(fn=classify_single, elems=picture_sum, fn_output_signature=tf.TensorSpec(shape=[1], dtype=tf.string))

        # Use "Classes" as the key to match the "Classify" output SignatureDef expectation
        # https://github.com/tensorflow/tensorflow/blob/86c79e9779a2d2653fddbb0e0391e2ea8121e81c/tensorflow/python/saved_model/signature_def_utils_impl.py#L155
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
    example_hotdog_proto = example_hotdog_proto.SerializeToString()

    example_no_hotdog_proto = tf.train.Example(
        features = tf.train.Features(feature={
            'picture': tf.train.Feature(float_list=tf.train.FloatList(value=tf.zeros((28*28,))))
        })
    )
    example_no_hotdog_proto = example_no_hotdog_proto.SerializeToString()

    example_batch = tf.constant([example_hotdog_proto, example_no_hotdog_proto])

    result = model.func(example_batch)
    # This should say "HotDog"
    print(result)

    tf.saved_model.save(model, 'models/classify_model/1', signatures={
        'classify_signature': model.func, 
    })

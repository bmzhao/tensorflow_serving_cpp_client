import tensorflow as tf

class XPlusOne(tf.Module):
    @tf.function(input_signature=[tf.TensorSpec(shape=[], dtype=tf.int32)])
    def func(self, x):
        return {
            'result': tf.add(x, tf.constant(1))
        }

if __name__ == '__main__':
    model = XPlusOne()
    tf.saved_model.save(model, 'examples/x_plus_one', signatures={
        'predict': model.func, 
    })

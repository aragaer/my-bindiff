import os
import subprocess

from nose.tools import eq_, ok_

def prepare_file(path, data):
    with open(path, "w") as out:
        out.write(data)

@given(u'{filename} is empty')
def step_impl(context, filename):
    prepare_file(os.path.join(context.dir, filename), '')


@given(u'{filename} contains the following')
def step_impl(context, filename):
    prepare_file(os.path.join(context.dir, filename),
                 context.text)


@when(u'I compare {filename1} and {filename2}')
def step_impl(context, filename1, filename2):
    result = subprocess.run([context.bindiff, filename1, filename2], cwd=context.dir, stdout=subprocess.PIPE)
    context.result = result.stdout.decode().strip()


@then(u'result is the following')
def step_impl(context):
    eq_(context.result, context.text)


@then(u'result starts with the following')
def step_impl(context):
    eq_(context.result[0:len(context.text)], context.text)

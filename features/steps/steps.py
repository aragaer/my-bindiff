import os
import subprocess

from nose.tools import eq_, ok_


@given(u'{filename} is empty')
def step_impl(context, filename):
    with open(os.path.join(context.dir, filename), "w") as out:
        pass


@when(u'I compare {filename1} and {filename2}')
def step_impl(context, filename1, filename2):
    result = subprocess.run([context.bindiff, filename1, filename2], cwd=context.dir, stdout=subprocess.PIPE)
    context.result = result.stdout.decode().strip()


@then(u'result is that files are identical')
def step_impl(context):
    expected = "are equal."
    eq_(context.result[-len(expected):], expected)

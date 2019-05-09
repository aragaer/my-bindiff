import os
import shutil
import tempfile


def before_all(context):
    context.bindiff = os.path.join(os.getcwd(), "my-bindiff")
    assert(os.path.exists(context.bindiff))


def before_scenario(context, scenario):
    context.dir = tempfile.mkdtemp()


def after_scenario(context, scenario):
    shutil.rmtree(context.dir)

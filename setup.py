from setuptools import setup
from setuptools import Extension
from Cython.Build import cythonize

setup(
    ext_modules=cythonize(
        [
            Extension(
                "A_Star",
                ["A_Star.pyx"],
                extra_compile_args=["-DNPY_NO_DEPRECATED_API"],
            )#TODO: add a comma here after completing CBS implementation
            # Extension(
            #     "libCBS",
            #     ["Convert_Map.pyx"],
            #     extra_compile_args=["-DNPY_NO_DEPRECATED_API"],
            # )
        ],
        annotate=True,
    )
)

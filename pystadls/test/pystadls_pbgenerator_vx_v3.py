import unittest
import inspect
from typing import get_type_hints
from pystadls_vx_v3 import PlaybackGenerator, generate, \
    SystemInit, ASICAdapterBoardInit, \
    CubeASICAdapterBoardInit, JboaASICAdapterBoardInit, \
    ChipInit
from pyhxcomm_vx import ZeroMockEntry


class PlaybackGeneratorTest(unittest.TestCase):
    @unittest.skip("Not implemented yet, see Issue #3395")
    def test_is_abstract(self):
        """
        Check if PlaybackGenerator is an abstract class.
        """
        self.assertTrue(inspect.isabstract(PlaybackGenerator),
                        "'PlaybackGenerator' is not abstract.")

    @unittest.skip("Not implemented yet, see Issue #3395")
    def test_abstract_generate(self):
        """
        Check if PlaybackGenerator.generate exists as an annotated abstract
        method.
        """
        self.assertTrue(
            getattr(PlaybackGenerator.generate, "__isabstractmethod__", False),
            "'PlaybackGenerator.generate' does not exist or is not abstract."
        )
        ret_type = get_type_hints(PlaybackGenerator.generate)["return"]
        self.assertTrue(
            issubclass(ret_type, tuple),
            "PlaybackGenerator.generate return type should be "
            f"'tuple', is {ret_type}"
        )

    def test_implementable(self):
        """
        Check if PlaybackGenerator can be implemented in python.
        """

        class MyGenerator(PlaybackGenerator):
            def generate(self) -> tuple:
                return tuple()

        self.assertEqual(tuple(), generate(MyGenerator()),
                         "Generated return value does not match expected.")

    def test_cpp_sequence(self):
        """
        Check that C++ sequences are usable in python.
        """

        for generator in [SystemInit(ZeroMockEntry()),
                          ASICAdapterBoardInit(),
                          CubeASICAdapterBoardInit(),
                          JboaASICAdapterBoardInit(),
                          ChipInit(ZeroMockEntry())]:
            self.assertTrue(isinstance(generator, PlaybackGenerator),
                            f"Instances of {str(generator)} are not of type "
                            f"'PlaybackGenerator' but {type(generator)}")

            self.assertTrue(isinstance(generate(generator),
                                       tuple),
                            f"'generate({str(generator)})' does not "
                            "produce results of type 'tuple'.")


if __name__ == '__main__':
    unittest.main()

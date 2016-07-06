import unittest

import odil

class TestDataSet(unittest.TestCase):
    def test_empty_constructor(self):
        data_set = odil.DataSet()
        self.assertTrue(data_set.empty())
        self.assertEqual(data_set.size(), 0)
        self.assertEqual(len(data_set), 0)
        self.assertEqual(len(data_set.get_transfer_syntax()), 0)

    def test_transfer_syntax_constructor(self):
        data_set = odil.DataSet(odil.registry.ExplicitVRLittleEndian)
        self.assertEqual(
            data_set.get_transfer_syntax(),
            odil.registry.ExplicitVRLittleEndian)

    def test_transfer_syntax(self):
        data_set = odil.DataSet()
        data_set.set_transfer_syntax(odil.registry.ExplicitVRLittleEndian)
        self.assertEqual(
            data_set.get_transfer_syntax(),
            odil.registry.ExplicitVRLittleEndian)

    def test_empty_element_tag(self):
        tag = odil.registry.PatientName
        data_set = odil.DataSet()
        data_set.add(tag)

        self.assertFalse(data_set.empty())
        self.assertEqual(data_set.size(), 1)
        self.assertEqual(len(data_set), 1)

        self.assertEqual(data_set.get_vr(tag), odil.VR.PN)
        self.assertTrue(data_set.empty(tag))
        self.assertEqual(data_set.size(tag), 0)

    def test_int_element(self):
        tag = odil.registry.SelectorUSValue
        value = [1, 2, 3]
        data_set = odil.DataSet()
        data_set.add(tag, value)

        self.assertFalse(data_set.empty())
        self.assertEqual(data_set.size(), 1)
        self.assertEqual(len(data_set), 1)

        self.assertEqual(data_set.get_vr(tag), odil.VR.US)
        self.assertFalse(data_set.empty(tag))
        self.assertEqual(data_set.size(tag), 3)
        self.assertTrue(data_set.is_int(tag))
        self.assertSequenceEqual(data_set.as_int(tag), value)

        value = [4, 5]
        data_set.set(tag, value)

        self.assertFalse(data_set.empty())
        self.assertEqual(data_set.size(), 1)
        self.assertEqual(len(data_set), 1)

        self.assertEqual(data_set.get_vr(tag), odil.VR.US)
        self.assertFalse(data_set.empty(tag))
        self.assertEqual(data_set.size(tag), len(value))
        self.assertTrue(data_set.is_int(tag))
        self.assertSequenceEqual(data_set.as_int(tag), value)

    def test_real_element(self):
        tag = odil.registry.SelectorFLValue
        value = [1.1, 2, 3.3]
        data_set = odil.DataSet()
        data_set.add(tag, value)

        self.assertFalse(data_set.empty())
        self.assertEqual(data_set.size(), 1)
        self.assertEqual(len(data_set), 1)

        self.assertEqual(data_set.get_vr(tag), odil.VR.FL)
        self.assertFalse(data_set.empty(tag))
        self.assertEqual(data_set.size(tag), 3)
        self.assertTrue(data_set.is_real(tag))
        self.assertSequenceEqual(data_set.as_real(tag), value)

        value = [4.4, 5]
        data_set.set(tag, value)

        self.assertFalse(data_set.empty())
        self.assertEqual(data_set.size(), 1)
        self.assertEqual(len(data_set), 1)

        self.assertEqual(data_set.get_vr(tag), odil.VR.FL)
        self.assertFalse(data_set.empty(tag))
        self.assertEqual(data_set.size(tag), len(value))
        self.assertTrue(data_set.is_real(tag))
        self.assertSequenceEqual(data_set.as_real(tag), value)

    def test_string_element(self):
        tag = odil.registry.SelectorCSValue
        value = ["foo", "bar"]
        data_set = odil.DataSet()
        data_set.add(tag, value)

        self.assertFalse(data_set.empty())
        self.assertEqual(data_set.size(), 1)
        self.assertEqual(len(data_set), 1)

        self.assertEqual(data_set.get_vr(tag), odil.VR.CS)
        self.assertFalse(data_set.empty(tag))
        self.assertEqual(data_set.size(tag), 2)
        self.assertTrue(data_set.is_string(tag))
        self.assertSequenceEqual(data_set.as_string(tag), value)

        value = ["baz"]
        data_set.set(tag, value)

        self.assertFalse(data_set.empty())
        self.assertEqual(data_set.size(), 1)
        self.assertEqual(len(data_set), 1)

        self.assertEqual(data_set.get_vr(tag), odil.VR.CS)
        self.assertFalse(data_set.empty(tag))
        self.assertEqual(data_set.size(tag), len(value))
        self.assertTrue(data_set.is_string(tag))
        self.assertSequenceEqual(data_set.as_string(tag), value)

    def test_data_set_element(self):
        tag = odil.registry.SelectorCodeSequenceValue
        value = [odil.DataSet(), odil.DataSet()]
        data_set = odil.DataSet()
        data_set.add(tag, value)

        self.assertFalse(data_set.empty())
        self.assertEqual(data_set.size(), 1)
        self.assertEqual(len(data_set), 1)

        self.assertEqual(data_set.get_vr(tag), odil.VR.SQ)
        self.assertFalse(data_set.empty(tag))
        self.assertEqual(data_set.size(tag), 2)
        self.assertTrue(data_set.is_data_set(tag))
        self.assertSequenceEqual(data_set.as_data_set(tag), value)

        value = [odil.DataSet()]
        data_set.set(tag, value)

        self.assertFalse(data_set.empty())
        self.assertEqual(data_set.size(), 1)
        self.assertEqual(len(data_set), 1)

        self.assertEqual(data_set.get_vr(tag), odil.VR.SQ)
        self.assertFalse(data_set.empty(tag))
        self.assertEqual(data_set.size(tag), len(value))
        self.assertTrue(data_set.is_data_set(tag))
        self.assertSequenceEqual(data_set.as_data_set(tag), value)

    def test_binary_element(self):
        tag = odil.registry.RedPaletteColorLookupTableData
        value = [bytearray("\x01\x02\x03")]
        data_set = odil.DataSet()
        data_set.add(tag, value)

        self.assertFalse(data_set.empty())
        self.assertEqual(data_set.size(), 1)
        self.assertEqual(len(data_set), 1)

        self.assertEqual(data_set.get_vr(tag), odil.VR.OW)
        self.assertFalse(data_set.empty(tag))
        self.assertEqual(data_set.size(tag), 1)
        self.assertTrue(data_set.is_binary(tag))
        self.assertSequenceEqual(
            [bytearray([x for x in item]) for item in data_set.as_binary(tag)],
            value)

        value = [bytearray("\x04\x05")]
        data_set.set(tag, value)

        self.assertFalse(data_set.empty())
        self.assertEqual(data_set.size(), 1)
        self.assertEqual(len(data_set), 1)

        self.assertEqual(data_set.get_vr(tag), odil.VR.OW)
        self.assertFalse(data_set.empty(tag))
        self.assertEqual(data_set.size(tag), len(value))
        self.assertTrue(data_set.is_binary(tag))
        self.assertSequenceEqual(
            [bytearray([x for x in item]) for item in data_set.as_binary(tag)],
            value)

    def test_getitem(self):
        data_set = odil.DataSet()
        data_set.add("PatientName", ["Doe^John"])
        self.assertSequenceEqual(
            data_set["PatientName"].as_string(), ["Doe^John"])
        self.assertRaises(Exception, lambda x: data_set["PatientID"])

    def test_iter(self):
        data_set = odil.DataSet()
        data_set.add("PatientName", ["Doe^John"])
        data_set.add("PatientID", ["DJ123"])
        self.assertEqual(
            [x.get_name() for x in data_set],
            ["PatientName", "PatientID"])

    def test_keys(self):
        data_set = odil.DataSet()
        data_set.add("PatientName", ["Doe^John"])
        data_set.add("PatientID", ["DJ123"])
        self.assertEqual(
            [x.get_name() for x in data_set.keys()],
            ["PatientName", "PatientID"])

    def test_values(self):
        data_set = odil.DataSet()
        data_set.add("PatientName", ["Doe^John"])
        data_set.add("PatientID", ["DJ123"])
        self.assertEqual(
            [
                [item for item in element.as_string()]
                for element in data_set.values()],
            [["Doe^John"], ["DJ123"]])

    def test_items(self):
        data_set = odil.DataSet()
        data_set.add("PatientName", ["Doe^John"])
        data_set.add("PatientID", ["DJ123"])
        self.assertSequenceEqual(
            [
                [tag.get_name(), [item for item in element.as_string()]]
                for tag, element in data_set.items()],
            [["PatientName", ["Doe^John"]], ["PatientID", ["DJ123"]]])

if __name__ == "__main__":
    unittest.main()

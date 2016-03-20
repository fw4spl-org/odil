import unittest

import _odil

class TestAssociationParameters(unittest.TestCase):
    def test_default_constructor(self):
        parameters = _odil.AssociationParameters()
        self.assertEqual(parameters.get_called_ae_title(), "")
        self.assertEqual(parameters.get_calling_ae_title(), "")
        self.assertEqual(len(parameters.get_presentation_contexts()), 0)

        user_identity = parameters.get_user_identity()
        self.assertEqual(
            user_identity.type, 
            _odil.AssociationParameters.UserIdentity.Type.None)

        self.assertEqual(parameters.get_maximum_length(), 16384)

    def test_called_ae_title(self):
        parameters = _odil.AssociationParameters()
        parameters.set_called_ae_title("foo")
        self.assertEqual(parameters.get_called_ae_title(), "foo")

    def test_calling_ae_title(self):
        parameters = _odil.AssociationParameters()
        parameters.set_calling_ae_title("foo")
        self.assertEqual(parameters.get_calling_ae_title(), "foo")

    def test_presentation_contexts(self):
        presentation_context = _odil.AssociationParameters.PresentationContext()
        presentation_context.id = 1
        presentation_context.abstract_syntax = "foo"
        presentation_context.transfer_syntaxes.append("bar")

        presentation_contexts = _odil.AssociationParameters.VPresentationContext()
        presentation_contexts.append(presentation_context)

        parameters = _odil.AssociationParameters()
        parameters.set_presentation_contexts(presentation_contexts)

        self.assertEqual(len(parameters.get_presentation_contexts()), 1)
        self.assertEqual(
            parameters.get_presentation_contexts()[0], presentation_context)

    def test_user_identity_username(self):
        parameters = _odil.AssociationParameters()
        parameters.set_user_identity_to_username("foo")

        user_identity = parameters.get_user_identity()
        self.assertEqual(
            user_identity.type, 
            _odil.AssociationParameters.UserIdentity.Type.Username)
        self.assertEqual(user_identity.primary_field, "foo")

    def test_user_identity_username_and_password(self):
        parameters = _odil.AssociationParameters()
        parameters.set_user_identity_to_username_and_password("foo", "bar")

        user_identity = parameters.get_user_identity()
        self.assertEqual(
            user_identity.type, 
            _odil.AssociationParameters.UserIdentity.Type.UsernameAndPassword)
        self.assertEqual(user_identity.primary_field, "foo")
        self.assertEqual(user_identity.secondary_field, "bar")

    def test_user_identity_kerberos(self):
        parameters = _odil.AssociationParameters()
        parameters.set_user_identity_to_kerberos("foo")

        user_identity = parameters.get_user_identity()
        self.assertEqual(
            user_identity.type, 
            _odil.AssociationParameters.UserIdentity.Type.Kerberos)
        self.assertEqual(user_identity.primary_field, "foo")

    def test_user_identity_saml(self):
        parameters = _odil.AssociationParameters()
        parameters.set_user_identity_to_saml("foo")

        user_identity = parameters.get_user_identity()
        self.assertEqual(
            user_identity.type, 
            _odil.AssociationParameters.UserIdentity.Type.SAML)
        self.assertEqual(user_identity.primary_field, "foo")

    def test_user_identity_none(self):
        parameters = _odil.AssociationParameters()
        parameters.set_user_identity_to_saml("foo")
        parameters.set_user_identity_to_none()

        user_identity = parameters.get_user_identity()
        self.assertEqual(
            user_identity.type, 
            _odil.AssociationParameters.UserIdentity.Type.None)

    def test_maximum_length(self):
        parameters = _odil.AssociationParameters()
        parameters.set_maximum_length(12345)
        self.assertEqual(parameters.get_maximum_length(), 12345)

    def test_chaining(self):
        parameters = _odil.AssociationParameters()
        parameters.set_called_ae_title("foo").set_calling_ae_title("bar")
        self.assertEqual(parameters.get_called_ae_title(), "foo")
        self.assertEqual(parameters.get_calling_ae_title(), "bar")

if __name__ == "__main__":
    unittest.main()


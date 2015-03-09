using Nancy;
using Nancy.Authentication.Forms;
using Nancy.Bootstrapper;
using Nancy.Conventions;
using Nancy.Cryptography;
using Nancy.TinyIoc;

namespace Server
{
    public class NancyBootstrapper : DefaultNancyBootstrapper
    {
        protected override void ApplicationStartup(TinyIoCContainer container, IPipelines pipelines)
        {
            var cryptographyConfiguration = new CryptographyConfiguration(
            new RijndaelEncryptionProvider(new PassphraseKeyGenerator("AutoHandWashSecretPass", new byte[] { 1, 2, 3, 4, 5, 6, 7, 8 })),
            new DefaultHmacProvider(new PassphraseKeyGenerator("AutoHandWashSuperSecure", new byte[] { 1, 2, 3, 4, 5, 6, 7, 8 })));
            
            base.ApplicationStartup(container, pipelines);
            var formsAuthConfiguration =
            new FormsAuthenticationConfiguration()
            {
                CryptographyConfiguration = cryptographyConfiguration,
                RedirectUrl = "~/login",
                UserMapper = container.Resolve<IUserMapper>()
            };
            FormsAuthentication.Enable(pipelines, formsAuthConfiguration);
        }

        protected override void ConfigureRequestContainer(TinyIoCContainer container, NancyContext context)
        {
            base.ConfigureRequestContainer(container, context);
            container.Register<IUserMapper, UserMapper>();
        }

        protected override void ConfigureConventions(NancyConventions nancyConventions)
        {
            base.ConfigureConventions(nancyConventions);
            nancyConventions.StaticContentsConventions.Add(StaticContentConventionBuilder.AddDirectory("Scripts"));
        }
    }
}

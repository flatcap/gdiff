#include <gnome.h>
#include "derived.h"

static void gtk_richard_init		(GtkRichard * richard);
static void gtk_richard_class_init	(GtkRichardClass * class);
static void gtk_richard_finalize	(GtkObject * object);

//______________________________________________________________________________
//
// Public
//

guint 
gtk_richard_get_type (void)
{
	static guint richard_type = 0;
	static const GtkTypeInfo richard_info =
	{
		"GtkRichard",
		sizeof (GtkRichard),
		sizeof (GtkRichardClass),
		(GtkClassInitFunc)  gtk_richard_class_init,
		(GtkObjectInitFunc) gtk_richard_init,
		/* reserved */ NULL,
		/* reserved */ NULL,
		(GtkClassInitFunc) NULL,
	};

	g_print ("gtk_richard_get_type\n");
	if (!richard_type)
	{
		richard_type = gtk_type_unique (gtk_widget_get_type (), &richard_info);
	}

	return richard_type;
}

GtkWidget *
gtk_richard_new (void)
{
	g_print ("gtk_richard_new\n");
	return gtk_widget_new(GTK_TYPE_RICHARD, NULL);
}

//______________________________________________________________________________
//
// Private

static void
gtk_richard_init (GtkRichard * richard)
{
	// initialise values
	richard->value = 42;
	
	g_print ("gtk_richard_init\n");
}

static void
gtk_richard_class_init (GtkRichardClass * class)
{
	// override methods
	GtkObjectClass *object_class = (GtkObjectClass*) class;

	object_class->finalize = gtk_richard_finalize;

	g_print ("gtk_richard_class_init\n");
}

static void
gtk_richard_finalize (GtkObject *object)
{
	GtkRichard	*richard = GTK_RICHARD (object);
	GtkObjectClass	*parent  = gtk_type_class (gtk_widget_get_type());	// my parent class

	g_print ("gtk_richard_finalize (value = %d)\n", richard->value);

	parent->finalize (object);
}


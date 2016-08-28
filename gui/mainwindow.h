#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

#include "NetworkInstance.h"

namespace Ui {
class MainWindow;
}

class QLabel;

class Config;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * Constructor of the Mainindow. Initialises the visibility of GUI components
     *
     * @param config The config object from which settings should be loaded.
     * @param parent Pointer to a parent widget.
     */
    MainWindow(Config* config, QWidget *parent = 0);

    /**
     * @brief ~MainWindow()
     * Destructor of MainWindow
     */
    ~MainWindow();

private:

    /**
     * Adds all queries contained in the QueryManager of the current NetworkInstance to the QueryHistory ListWidget
     *
     * @param net Pointer to the NetworkInstance
     */
    void loadQueriesToHistoryWindow(const NetworkInstance* net);

    /**
     * Adapts the buttons controlling loading of previous and subsequent query items depending on the current query index
     */
    void checkQueriesLeft();

    /**
     * @brief adaptQueryEvaluationButtons
     * @param show Flag controlling the appearance of all button related to query evaluation
     */
    void adaptQueryEvaluationButtons(bool show);

    /**
     * @brief initaliseVisibility
	 * Initialises the visibility of the GUI components at the start of the programme
     */
    void initaliseVisibility();

    /**
	 * Trains the network according to the provided samples and the provided discretisation information.
     *
     * @param samples File containing the samples.
     * @param control File containing the discretisation information.
     * @param index Index of the NetworkInstance
     */
    void loadSamples(const QString& samples, const QString& control, unsigned int index);

    /**
	 * Create the network visualisation of the given network instance
     * @param index Index of the NetworkInstance
     */
    void visualise(int index);

    /**
     * @brief loadSif
	 * Loads a file in the SimpleInteractionFormat
     * @param filename Name of the file 
     * @param index Index of the NetworkInstance
     */
    void loadSif(QString filename, int index);

    /**
     * @brief loadNAorTGF
	 * Loads a NA or TGF file
     * @param filename Name of the file
     * @param index Index of the NetworkInstance
     */
    void loadNAorTGF(QString filename, int index);

    /**
     * @brief generateNetworkInstance
	 * Generates a new NetworkInstance object
     * @return The index of the generated NetworkInstance
     */
    int generateNetworkInstance();

	int addNetwork(NetworkInstance* network);

	/**
	 * This method adapts the various GUI elements after samples
	 * have been loaded for a network.
	 *
	 * @param network The NetworkInstance for which the samples have
	 * been loaded.
	 */
	void samplesLoaded(NetworkInstance* network);

    /**
     * @brief ui
	 * The Graphical User Interface
     */
    Ui::MainWindow *ui;

    Config* config_;

private slots:
	void addLogMessage(const QString&);
	void queryExecuted(unsigned int);

	void addEdgeSelected();
	void removeEdgeSelected();
	void conditionValueSelected(QAction*);
	void interventionValueSelected(QAction*);

    /**
     * @param samples Name of the file containing the original observations
     * @param deselected The list of samples that should not be considered.
     */
    void discretiseSelection(const QString& samples, const std::vector<uint>& deselected);

    /**
     * Calls the loadSamples method
     */
    void on_actionLoad_Samples_triggered();

    /**
     * @brief on_actionAbout_triggered
	 * Shows information on Causal Trail
     */
    void on_actionAbout_triggered();

    /**
     * @brief on_actionHelp_triggered
	 * Shows the documentation of Causal Trails GUI
     */
    void on_actionHelp_triggered();

    /**
     * @brief on_tabWidget_tabCloseRequested
	 * Closes the tab with the given index and removes the corresponding NetworkInstance
     * @param index Index of the closed tab
     */
    void on_tabWidget_tabCloseRequested(int index);

    /**
	 * @brief on_actionDeleteNetwork_triggered
	 * Deletes the current network
     */
    void on_actionDeleteNetwork_triggered();

    /**
     * @brief on_actionNewNetwork_triggered
	 * Loads a new network
     */
    void on_actionLoadNetwork_triggered();

    /**
     * @brief on_tabWidget_currentChanged
	 * Changes the network visualization, adapts the query history and all components related to query evaulation
     * @param index Index of tabTabwidget that is now selectet
     */
    void on_tabWidget_currentChanged(int index);

    /**
     * @brief on_actionLayout_triggered
	 * Relayouts the network
     */
    void on_actionLayout_triggered();

    /**
	 * Handles a double click on a node. 
     *
     * @param node The node that was double-clicked
     */
    void Node_double_clicked(NodeGui* node);

    /**
     * Handles a right click on a node, generates the context menu
     *
     * @param node The node for which the context menu was requested
     * @param event QGraphicsSceneContextMenuEvent Pointer
     */
    void Node_context(NodeGui* node, QContextMenuEvent* event);

    /**
     * @brief Edge_context
	 * Handles a right click on an edge, generates the context menu
     * @param edge The edge for which the context menu was invoked
     * @param event QGraphicsSceneContextMenuEvent Pointer
     */
    void Edge_context(Edge* edge, QContextMenuEvent *event);

    /**
     * @brief context_Menu_QueryValue_Selected
	 * Adds the selected query value to the non intervention list widget
     * @param act QAction Pointer
     */
    void context_Menu_QueryValue_Selected(QAction* act);

    /**
     * @brief context_Menu_ArgMax_Selected
	 * Adds the selected query value to the non intervention list widget. Sets the current query to be a MAP query.
     */
    void context_Menu_ArgMax_Selected();

    /**
     * @brief context_Menu_ShowMatrix_selected
	 * Displays the probability matrix of the selected node
     */
    void context_Menu_ShowMatrix_selected();

    /**
     * @brief on_actionSaveSession_triggered
	 * Saves the current session
     */
    void on_actionSave_Session_triggered();

    /**
     * @brief on_actionLoad_Session_triggered
	 * Loads a previously saved session
     */
    void on_actionLoad_Session_triggered();

    /**
     * @brief on_actionCreate_Batchfile_triggered
	 * Creates a batchfile containing all queries that are in the current query history
     */
    void on_actionCreate_Batchfile_triggered();

    /**
     * @brief on_actionExecute_Batchfile_triggered
	 * Executes a previously saved batchile
     */
    void on_actionExecute_Batchfile_triggered();

	void on_actionExportSvg_triggered();

	void on_queryHistory_doubleClicked(const QModelIndex& index);

	/**
	 * @brief dataRejected
	 * Resets the network if the user did not select data in the DataView
	 */
    void dataRejected();

    /**
     * @brief on_actionSave_discretised_data_triggered
     * Writes the discretised data in a user specified txt file
     */
    void on_actionSave_discretised_data_triggered();

private:
	NetworkInstance* currentNetwork_();
	NetworkInstance* getNetwork_(int index);
};

#endif // MAINWINDOW_H
